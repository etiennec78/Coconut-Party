#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "terrain.h"

void freeTerrain(char** terrain) {
    if (terrain != NULL) {
        if (terrain[0] != NULL) {
            free(terrain[0]);
        }
        free(terrain);
    }
}

char** allocateTerrain(int width, int height) {
    char* data = malloc(width * height * sizeof(char));
    if (data == NULL) {
        printf("Error: Failed to allocate terrain !\n");
        exit(1);
    }

    char** terrain = malloc(width * sizeof(char*));
    if (terrain == NULL) {
        free(data);
        printf("Error: Failed to allocate terrain !\n");
        exit(1);
    }

    for (int x = 0; x < width; x++) {
        terrain[x] = data + (x * height);
    }

    return terrain;
}

Coordinates findStart(Game* game) {
    Coordinates start;
    start.x = game->data.width / 2;
    start.y = game->data.height - 1;

    while (start.y >= 0 && game->terrain[start.x][start.y] == 2) {
        start.y--;
    }
    return start;
}

int isValidEnd(Coordinates coordinates, char** terrain) {
    int y = coordinates.y;
    // If the path tile reached the top, consider it valid
    if (y == 0) {
        return 1;
    }

    // If water is above, consider it valid
    int x = coordinates.x;
    return terrain[x][y-1] == 2 || y == 0; // 2 = Water
}

void constructPath(Path* path, int size) {
    path->tab = malloc(size * sizeof(Coordinates));
    if (path->tab == NULL) {
        printf("Failed to allocate path !\n");
        exit(1);
    }
    path->length = 0;
}

Path copyPath(Path path, int size) {
    Path newPath;
    constructPath(&newPath, size);

    for (int i = 0; i < path.length; i++) {
        newPath.tab[i] = path.tab[i];
    }
    newPath.length = path.length;

    return newPath;
}

void shuffleCoords(Coordinates* list, int N) {
    for (int i = 0; i < N-1; ++i) {
        int j = rand() % (N-i) + i;
        Coordinates temp = list[i];
        list[i] = list[j];
        list[j] = temp;
    }
}

int coordsInPath(int x, int y, Path path) {
    for (int i = 0; i < path.length; i++) {
        if (path.tab[i].x == x && path.tab[i].y == y) {
            return 1;
        }
    }
    return 0;
}

int validatePathTileChoice(Game* game, Path path, Coordinates current, Coordinates next) {
    Coordinates surrounding;

    // Return false if the next path tile is outside of the terrain
    if (next.x < 0 || next.x > game->data.width - 1
        || next.y < 0 || next.y > game->data.height - 1
    ) {
        return 0;
    }

    // Return false if the next path tile is not land
    if (game->terrain[next.x][next.y] > 1) {
        return 0;
    }

    // Return false if the next path tile won't be able to reach the top
    int caseNeeded = next.y;
    int caseRemaining = game->data.maxPathLength - path.length + 1;
    if (caseNeeded > caseRemaining) {
        return 0;
    }

    // Return false if there is already a path tile there
    if (coordsInPath(next.x, next.y, path)) {
        return 0;
    }

    // Return false if the next path tile is surrounded by other path tiles
    for (int i = 0; i < 4; i++) {
        surrounding = next;
        switch (i) {
            case 0:
                surrounding.x = next.x-1;
                break;
            case 1:
                surrounding.x = next.x+1;
                break;
            case 2:
                surrounding.y = next.y-1;
                break;
            case 3:
                surrounding.y = next.y+1;
                break;
        }
        if (
            // Don't exit the terrain to avoid segfault
            surrounding.x >= 0 && surrounding.x < game->data.width
            && surrounding.y >= 0 && surrounding.y < game->data.height

            // Don't take into account the current path
            && !(surrounding.x == current.x && surrounding.y == current.y)
        ) {
            // If surrounding coordinates are in the path list, return false
            if (coordsInPath(surrounding.x, surrounding.y, path)) {
                return 0;
            }
        }
    }

    return 1;
}

Coordinates* findAvailablePathTiles(Game* game, Path currentPath, Coordinates currentCoordinates, int* availableLength) {
    Coordinates* available = malloc(4 * sizeof(Coordinates));
    if (available == NULL) {
        printf("Failed to allocate available coordinates list !\n");
        exit(1);
    }

    Coordinates nextCoordinates;
    for (int i = 0; i < 4; i++) {
        nextCoordinates = currentCoordinates;
        switch (i) {
            case 0:
                nextCoordinates.x = currentCoordinates.x-1;
                break;
            case 1:
                nextCoordinates.x = currentCoordinates.x+1;
                break;
            case 2:
                nextCoordinates.y = currentCoordinates.y-1;
                break;
            case 3:
                nextCoordinates.y = currentCoordinates.y+1;
                break;
        }
        if (validatePathTileChoice(game, currentPath, currentCoordinates, nextCoordinates)) {
            available[*availableLength] = nextCoordinates;
            (*availableLength)++;
        }
    }

    return available;
}

Path findNextPath(Game* game, Path path, int* pathValid) {
    Coordinates currentCoordinates = path.tab[path.length - 1];

    // Stop condition : if the path is too long
    if (path.length + 1 > game->data.maxPathLength) {
        *pathValid = 0;
        return path;
    }

    int availableLength = 0;
    Coordinates* available = findAvailablePathTiles(game, path, currentCoordinates, &availableLength);

    // Stop condition : if there are no available directions
    if (availableLength == 0) {
        free(available);
        *pathValid = (
            isValidEnd(currentCoordinates, game->terrain)
            && path.length > game->data.minPathLength && path.length < game->data.maxPathLength
        );
        return path;
    }

    // Try to find a valid path in the available directions
    shuffleCoords(available, availableLength);
    Path nextPath;
    for (int i = 0; i < availableLength; i++) {
        nextPath = copyPath(path, game->data.width * game->data.height);
        nextPath.tab[nextPath.length] = available[i];
        nextPath.length++;

        // Stop condition : if the path is valid and long enough
        if (isValidEnd(available[i], game->terrain) && path.length > game->data.minPathLength) {
            free(available);
            *pathValid = 1;
            return nextPath;
        }

        nextPath = findNextPath(game, nextPath, pathValid);

        if (*pathValid) {
            free(available);
            return nextPath;
        }
        free(nextPath.tab);
    }
    free(available);

    // If all directions led to nowhere, return as invalid
    *pathValid = 0;
    return path;
}

Path generatePath(Game* game) {
    if (game->data.minPathLength > game->data.maxPathLength) {
        printf("Error: Maximum path length cannot be less than minimum path length.\n");
        exit(1);
    }
    if (game->data.maxPathLength < game->data.height) {
        printf("Error: The maximum path length is too short for this terrain.\n");
        exit(1);
    }
    // Find the maximum path length for this terrain size (S-shaped)
    float smallest;
    int biggest;
    if (game->data.width > game->data.height) {
        biggest = game->data.width;
        smallest = game->data.height;
    } else {
        biggest = game->data.height;
        smallest = game->data.width;
    }
    int maxPossibleLength = ceil(smallest/2)*biggest + floor(smallest/2);
    if (game->data.minPathLength > maxPossibleLength) {
        printf("Error: The minimum length is too high for this terrain.\n");
        exit(1);
    }

    Path path;
    constructPath(&path, game->data.width*game->data.height);
    path.tab[0] = findStart(game);
    path.length = 1;

    int pathValid = 0;
    Path finalPath = findNextPath(game, path, &pathValid);

    if (!pathValid || finalPath.length == 0) {
        free(finalPath.tab);
        printf("Error: No valid path could be found !\n");
        exit(1);
    }
    return finalPath;
}

void insertPath(char** terrain, Path path) {
    int i = 0;
    int first_x = path.tab[i].x;
    int first_y = path.tab[i].y;
    terrain[first_x][first_y] = 4; // Place start

    for (i=1; i < path.length - 1; i++) {
        Coordinates pathCoor = path.tab[i];
        terrain[pathCoor.x][pathCoor.y] = 3; // Place path tile
    }

    int last_x = path.tab[i].x;
    int last_y = path.tab[i].y;
    terrain[last_x][last_y] = 5; // Place crown
}

void createTerrain(Game* game) {
    if (game->data.width * game->data.height < 3) {
        printf("Error: Terrain needs at least 3 tiles.\n");
        exit(1);
    }

    char** terrain = allocateTerrain(game->data.width, game->data.height);
    srand(game->data.seed);

    for (int x = 0; x < game->data.width; x++) {
        for (int y = 0; y < game->data.height; y++) {
            switch (rand()%2) {
                case 0:
                    terrain[x][y] = 0;
                    break;
                case 1:
                    terrain[x][y] = 1;
                    break;
            }
        }
    }

    game->terrain = terrain;
    game->path = generatePath(game);
    insertPath(terrain, game->path);
}
