#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "terrain.h"
#include "common.h"

typedef struct {
    int axis;
    int direction;
} AxisVect;

typedef struct {
    Coordinates coord;
    AxisVect vect;
    int* coordAxis;
    int originAxis;
    int length;
    int stoppingReason;
} Ray;

int getMaxPathLength(Game* game) {
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
    return ceil(smallest/2)*biggest + floor(smallest/2);
}

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

char** copyTerrain(char** terrain, int width, int height) {
    char** new = allocateTerrain(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            new[i][j] = terrain[i][j];
        }
    }
    return new;
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

void constructPath(Game* game, Path* path) {
    path->tab = malloc(getMaxPathLength(game) * sizeof(Coordinates));
    if (path->tab == NULL) {
        printf("Failed to allocate path !\n");
        exit(1);
    }
    path->length = 0;
}

Path copyPath(Game* game, Path path) {
    Path newPath;
    constructPath(game, &newPath);

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

int getIndexAtCoordinates(Path path, Coordinates coord) {
    for (int i = 0; i < path.length; i++) {
        if (coordsEqual(path.tab[i], coord)) {
            return i;
        }
    }
    return -1;
}

int coordsInPath(Coordinates coord, Path path) {
    return getIndexAtCoordinates(path, coord) != -1;
}

AxisVect getAdjVect(Coordinates coord1, Coordinates coord2) {
    AxisVect vector;
    int x = coord2.x - coord1.x;
    if (x != 0) {
        vector.axis = 0;
        vector.direction = x;
    } else {
        int y = coord2.y - coord1.y;
        vector.axis = 1;
        vector.direction = y;
    }
    return vector;
}

int coordsInTerrain(Game* game, Coordinates coord) {
    return !(
        coord.x < 0 || coord.x > game->data.width - 1
        || coord.y < 0 || coord.y > game->data.height - 1
    );
}

void sendRay(Game* game, Path path, Ray* ray) {
    ray->stoppingReason = 0;
    do {
        *ray->coordAxis += ray->vect.direction;
        ray->length = abs(ray->originAxis - *ray->coordAxis);
        if (coordsInPath(ray->coord, path)) {
            ray->stoppingReason = 1;
            break;
        }
    } while (coordsInTerrain(game, ray->coord));

    *ray->coordAxis -= ray->vect.direction;
    ray->length = abs(ray->originAxis - *ray->coordAxis);
}

void configureExploringRays(AxisVect nextVect, Coordinates next, Ray* leftRay, Ray* rightRay, Ray* topRay) {
    leftRay->coord = rightRay->coord = topRay->coord = next;
    
    topRay->vect = nextVect;
    topRay->coordAxis = (nextVect.axis == 0) ? &topRay->coord.x : &topRay->coord.y;
    topRay->originAxis = (nextVect.axis == 0) ? next.x : next.y;
    
    int sideAxis = 1 - nextVect.axis;
    
    leftRay->vect.axis = rightRay->vect.axis = sideAxis;
    leftRay->coordAxis = (sideAxis == 0) ? &leftRay->coord.x : &leftRay->coord.y;
    rightRay->coordAxis = (sideAxis == 0) ? &rightRay->coord.x : &rightRay->coord.y;
    leftRay->originAxis = rightRay->originAxis = (sideAxis == 0) ? next.x : next.y;
    
    int dirFactor = (nextVect.direction == -1) ? 1 : -1;
    leftRay->vect.direction = dirFactor;
    rightRay->vect.direction = -dirFactor;
}

int cornerBlocked(Game* game, Path path, Coordinates current, AxisVect nextVect, Ray* oldRay) {
    Ray turnRay1, turnRay2, turnRay3;
    Ray* rays[3] = {&turnRay1, &turnRay2, &turnRay3};
    
    for (int i = 0; i < 3; i++) {
        rays[i]->coord = current;
        rays[i]->vect.axis = nextVect.axis;
        rays[i]->vect.direction = -nextVect.direction; // Go towards the back
    }
    
    int incrementAxis = oldRay->vect.axis;
    
    for (int i = 0; i < 3; i++) {
        if (incrementAxis == 0) {
            rays[i]->coord.x += i + 1;
            rays[i]->coordAxis = &rays[i]->coord.y;
        } else {
            rays[i]->coord.y += i + 1;
            rays[i]->coordAxis = &rays[i]->coord.x;
        }

        if (incrementAxis == 0) {
            rays[i]->originAxis = current.y;
        } else {
            rays[i]->originAxis = current.x;
        }
        
        sendRay(game, path, rays[i]);
        if (rays[i]->length < 3) {
            return 1;
        }
    }
    return 0;
}

int onEdge(Ray leftRay, Ray rightRay) {
    return (
        leftRay.stoppingReason == 0 && leftRay.length < 2
        || rightRay.stoppingReason == 0 && rightRay.length < 2
    );
}

int goingTowardsStart(AxisVect nextVect, Coordinates start, Coordinates next) {
    if (nextVect.axis == 1 && nextVect.direction == 1) {
        return 1;
    } else if (nextVect.axis == 0) {
        int startToNext = start.x - next.x;
        if (
            nextVect.direction == -1 && startToNext < 0
            || nextVect.direction == 1 && startToNext > 0
        ) {
            return 1;
        }
    }
    return 0;
}

int isDeadEnd(Game* game, Path path, Coordinates current, Coordinates next) {
    Ray leftRay, rightRay, topRay;
    AxisVect nextVect = getAdjVect(current, next);
    Coordinates start = path.tab[0];

    // Configure rays going on the left, on the right and above the next path tile
    configureExploringRays(nextVect, next, &leftRay, &rightRay, &topRay);

    // Send the rays until they hit the path or the edge
    Ray* rays[3] = {&leftRay, &rightRay, &topRay};
    for (int i = 0; i < 3; i++) {
        sendRay(game, path, rays[i]);
    }

    // If the next path tile is surrouned by paths, check that there is still a reachable exit towards the oldest path
    if (
        leftRay.stoppingReason == 1 && rightRay.stoppingReason == 1 && topRay.stoppingReason == 1
    ) {
        // Move left and right rays forward to get path tiles coordinates
        *leftRay.coordAxis += leftRay.vect.direction;
        *rightRay.coordAxis += rightRay.vect.direction;

        // Find the index of these tiles in the path list
        int indexLeft = getIndexAtCoordinates(path, leftRay.coord);
        int indexRight = getIndexAtCoordinates(path, rightRay.coord);

        // Find which ray reaches the oldest path tile
        Ray* oldRay;
        if (indexLeft < indexRight) {
            oldRay = &leftRay;
        } else {
            oldRay = &rightRay;
        }

        // If the exit gap is too narrow, and the path is currently going in the dead end, return true
        if (oldRay->length < 3 && (
                nextVect.axis != oldRay->vect.axis
                || nextVect.direction != oldRay->vect.direction
            )
        ) {
            return 1;
        }

        Coordinates previous = path.tab[path.length - 2];
        AxisVect currentVect = getAdjVect(previous, current);

        // If the path has turned, send three rays to also check that there is enough space on the corners
        if (currentVect.axis != nextVect.axis && cornerBlocked(game, path, current, nextVect, oldRay)) {
            return 1;
        }
    }

    // If the path is on the edge, only allow to go away from start to avoid dead ends
    else if (onEdge(leftRay, rightRay) && goingTowardsStart(nextVect, start, next)) {
        return 1;
    }

    return 0;
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

    // Return false if this direction will lead to a dead end
    if (isDeadEnd(game, path, current, next)) {
        free(surroundingTiles);
        return 0;
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
        nextPath = copyPath(game, path);
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
    int maxPossibleLength = getMaxPathLength(game);
    if (game->data.minPathLength > maxPossibleLength) {
        printf("Error: The minimum length is too high for this terrain.\n");
        exit(1);
    }

    Path path;
    constructPath(game, &path);
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
