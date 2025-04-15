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

int coordsEqual(Coordinates coord1, Coordinates coord2) {
    return coord1.x == coord2.x && coord1.y == coord2.y;
}

int coordsInPath(Coordinates coord, Path path) {
    for (int i = 0; i < path.length; i++) {
        if (coordsEqual(path.tab[i], coord)) {
            return 1;
        }
    }
    return 0;
}

int coordsInTerrain(Game* game, Coordinates coord) {
    return !(
        coord.x < 0 || coord.x > game->data.width - 1
        || coord.y < 0 || coord.y > game->data.height - 1
    );
}

Coordinates* getSurroundingTiles(Game* game, Coordinates currentCoordinates, int* surroundingLength) {
    Coordinates* surrounding = malloc(4 * sizeof(Coordinates));

    if (surrounding == NULL) {
        printf("Failed to allocate surrounding path tiles list !\n");
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

        if (!coordsInTerrain(game, nextCoordinates)) {
            continue;
        }

        surrounding[*surroundingLength] = nextCoordinates;
        (*surroundingLength)++;
    }

    return surrounding;
}

int validatePathTileChoice(Game* game, Path path, Coordinates current, Coordinates next) {

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
    if (coordsInPath(next, path)) {
        return 0;
    }


    // Return false if the next path tile is surrounded by other path tiles
    int surroundingLength = 0;
    Coordinates* surroundingTiles = getSurroundingTiles(game, next, &surroundingLength);

    for (int i = 0; i < surroundingLength; i++) {
        Coordinates surrounding = surroundingTiles[i];

        // Don't take into account the current path
        if (!(coordsEqual(surrounding, current))) {

            // If surrounding coordinates already are in the path, return false
            if (coordsInPath(surrounding, path)) {
                free(surroundingTiles);
                return 0;
            }

        }
    }

    free(surroundingTiles);
    return 1;
}

Path findNextPath(Game* game, Path path, int* pathValid) {
    Coordinates currentCoordinates = path.tab[path.length - 1];

    // Stop condition : if the path is too long
    if (path.length + 1 > game->data.maxPathLength) {
        *pathValid = 0;
        return path;
    }

    int surroundingLength = 0;
    Coordinates* surroundingTiles = getSurroundingTiles(game, currentCoordinates, &surroundingLength);
    shuffleCoords(surroundingTiles, surroundingLength);

    // Try to find a valid path in the available directions
    Path nextPath;
    for (int i = 0; i < surroundingLength; i++) {
        nextPath = copyPath(path, game->data.width * game->data.height);
        nextPath.tab[nextPath.length] = surroundingTiles[i];
        nextPath.length++;

        if (!validatePathTileChoice(game, path, currentCoordinates, surroundingTiles[i])) {
            free(nextPath.tab);
            continue;
        }

        if (isValidEnd(surroundingTiles[i], game->terrain) && nextPath.length > game->data.minPathLength) {
            free(surroundingTiles);
            free(path.tab);
            *pathValid = 1;
            return nextPath;
        }

        nextPath = findNextPath(game, nextPath, pathValid);

        if (*pathValid) {
            free(surroundingTiles);
            free(path.tab);
            return nextPath;
        }
        free(nextPath.tab);
    }
    free(surroundingTiles);

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
