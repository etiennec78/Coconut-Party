#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "terrain.h"
#include "common.h"
#include "monkeySlots.h"
#include "display.h"
#include "backgroundEntities.h"

typedef enum {
    AXIS_X = 0,
    AXIS_Y = 1
} Axis;

typedef enum {
    DIR_MINUS = -1,
    DIR_PLUS = 1
} Direction;

typedef struct {
    Axis axis;
    Direction direction;
} AxisVect;

typedef enum {
    STOP_OUT = 0,
    STOP_PATH = 1
} StoppingReason;

typedef struct {
    Coordinates coord;
    AxisVect vect;
    int* coordAxis;
    int originAxis;
    int length;
    StoppingReason stoppingReason;
} Ray;

typedef enum {
    RAY_LEFT = 0,
    RAY_RIGHT = 1,
    RAY_TOP = 2
} ExploringRay;

const float LAND_PROBA[6] = {
    // 35%, 35%, 20%, 10%
    0.65, 0.30, 0.1, 0
};

const float BACKGROUND_ENTITIES_PROBA[6] = {
    // Land: 1%, 3%
    // Water: 0.1%, 0.2%, 0.5%, 1%
    0.001, 0.003, 0.001, 0.002, 0.005, 0.01
};

int getMaxPathLength(Game* game) {
    // Find the approximative maximum path length for this terrain size (S-shaped)
    float smallest;
    int biggest;
    int landWidth = game->data.width * LAND_WATER_RATIO;
    int landHeight = game->data.height * LAND_WATER_RATIO;

    if (landWidth > landHeight) {
        biggest = landWidth;
        smallest = landHeight;
    } else {
        biggest = landHeight;
        smallest = landWidth;
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
    start.y = game->data.height / 2;

    while (start.y < game->data.height && game->terrain[start.x][start.y] != WATER) {
        start.y++;
    }
    start.y--;
    return start;
}

int isValidEnd(Game* game, Coordinates coordinates) {
    int y = coordinates.y;
    // If the path tile reached the top, consider it valid
    if (y == 0) {
        return 1;
    }

    // If water is above, consider it valid
    int x = coordinates.x;
    return y < game->data.endHeight && game->terrain[x][y-1] == WATER;
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
    for (int i = path.length - 1; i >= 0; i--) {
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
        vector.axis = AXIS_X;
        vector.direction = x;
    } else {
        int y = coord2.y - coord1.y;
        vector.axis = AXIS_Y;
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

int isWater(Game* game, Coordinates coord) {
    return game->terrain[coord.x][coord.y] == WATER;
}

Ray* findRayFromVect(AxisVect vect, Ray** rays, int N) {
    for (int i = 0; i < N; i++) {
        if (rays[i]->vect.axis == vect.axis && rays[i]->vect.direction == vect.direction) {
            return rays[i];
        }
    }
    return NULL;
}

void sendRay(Game* game, Path path, Ray* ray) {
    ray->stoppingReason = STOP_OUT;
    do {
        *ray->coordAxis += ray->vect.direction;
        ray->length = abs(ray->originAxis - *ray->coordAxis);
        if (coordsInPath(ray->coord, path)) {
            ray->stoppingReason = STOP_PATH;
            break;
        }
    } while (coordsInTerrain(game, ray->coord) && !isWater(game, ray->coord));

    *ray->coordAxis -= ray->vect.direction;
    ray->length = abs(ray->originAxis - *ray->coordAxis);
}

void configureExploringRays(AxisVect nextVect, Coordinates next, Ray** rays) {
    rays[RAY_LEFT]->coord = rays[RAY_RIGHT]->coord = rays[RAY_TOP]->coord = next;

    rays[RAY_TOP]->vect = nextVect;
    rays[RAY_TOP]->coordAxis = (nextVect.axis == AXIS_X) ? &rays[RAY_TOP]->coord.x : &rays[RAY_TOP]->coord.y;
    rays[RAY_TOP]->originAxis = (nextVect.axis == AXIS_X) ? next.x : next.y;

    int sideAxis = 1 - nextVect.axis;

    rays[RAY_LEFT]->vect.axis = rays[RAY_RIGHT]->vect.axis = sideAxis;
    rays[RAY_LEFT]->coordAxis = (sideAxis == AXIS_X) ? &rays[RAY_LEFT]->coord.x : &rays[RAY_LEFT]->coord.y;
    rays[RAY_RIGHT]->coordAxis = (sideAxis == AXIS_X) ? &rays[RAY_RIGHT]->coord.x : &rays[RAY_RIGHT]->coord.y;
    rays[RAY_LEFT]->originAxis = rays[RAY_RIGHT]->originAxis = (sideAxis == AXIS_X) ? next.x : next.y;

    int dirFactor = nextVect.direction;
    dirFactor *= (nextVect.axis == AXIS_X) ? -1 : 1;
    rays[RAY_LEFT]->vect.direction = dirFactor;
    rays[RAY_RIGHT]->vect.direction = -dirFactor;
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
        if (incrementAxis == AXIS_X) {
            rays[i]->coord.x += i + 1;
            rays[i]->coordAxis = &rays[i]->coord.y;
        } else {
            rays[i]->coord.y += i + 1;
            rays[i]->coordAxis = &rays[i]->coord.x;
        }

        if (incrementAxis == AXIS_X) {
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

int onEdge(Ray** rays) {
    // Don't consider up an edge since this is the finish line
    AxisVect left, right, down;

    left.axis = right.axis = AXIS_X;
    down.axis = AXIS_Y;
    left.direction = DIR_MINUS;
    right.direction = down.direction = DIR_PLUS;

    AxisVect vect[3] = {left, right, down};
    Ray* terrainRays[3] = {NULL, NULL, NULL};

    // Check for each side of the terrain if its ray is shorter than 2
    for (int i = 0; i < 3; i++) {
        terrainRays[i] = findRayFromVect(vect[i], rays, 3);
        if (terrainRays[i] != NULL && terrainRays[i]->stoppingReason == STOP_OUT && terrainRays[i]->length < 2) {
            return 1;
        }
    }
    return 0;
}

int goingTowardsStart(AxisVect nextVect, Coordinates start, Coordinates next) {
    if (nextVect.axis == AXIS_Y && nextVect.direction == DIR_PLUS) {
        return 1;
    } else if (nextVect.axis == AXIS_X) {
        int startToNext = start.x - next.x;
        if (
            nextVect.direction == DIR_MINUS && startToNext < 0
            || nextVect.direction == DIR_PLUS && startToNext > 0
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
    Ray* rays[3] = {&leftRay, &rightRay, &topRay};

    // Configure rays going on the left, on the right and above the next path tile
    configureExploringRays(nextVect, next, rays);

    // Send the rays until they hit the path or the edge
    for (int i = 0; i < 3; i++) {
        sendRay(game, path, rays[i]);
    }

    // If the next path tile is surrouned by paths, check that there is still a reachable exit towards the oldest path
    if (
        rays[RAY_LEFT]->stoppingReason == STOP_PATH && rays[RAY_RIGHT]->stoppingReason == STOP_PATH && rays[RAY_TOP]->stoppingReason == STOP_PATH
    ) {
        // Move left and right rays forward to get path tiles coordinates
        *rays[RAY_LEFT]->coordAxis += rays[RAY_LEFT]->vect.direction;
        *rays[RAY_RIGHT]->coordAxis += rays[RAY_RIGHT]->vect.direction;

        // Find the index of these tiles in the path list
        int indexLeft = getIndexAtCoordinates(path, rays[RAY_LEFT]->coord);
        int indexRight = getIndexAtCoordinates(path, rays[RAY_RIGHT]->coord);

        // Find which ray reaches the oldest path tile
        Ray* oldRay;
        if (indexLeft < indexRight) {
            oldRay = rays[RAY_LEFT];
        } else {
            oldRay = rays[RAY_RIGHT];
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
    else if (onEdge(rays) && goingTowardsStart(nextVect, start, next)) {
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
    if (game->terrain[next.x][next.y] < LAND_FIRST) {
        return 0;
    }

    // Return false if the next path tile won't be able to reach the top
    int tileNeeded = next.y;
    int tileRemaining = game->data.maxPathLength - path.length + 1;
    if (tileNeeded > tileRemaining) {
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

Path findNextPath(Game* game, Path path, int* pathValid, unsigned int startTime) {

    // Stop generating path if the process took too long
    if (time(NULL) - startTime > game->data.backoff.maxTime) {
        free(path.tab);
        Path nullPath;
        nullPath.tab = NULL;
        nullPath.length = 0;
        *pathValid = 0;
        return nullPath;
    }

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

        if (isValidEnd(game, surroundingTiles[i]) && nextPath.length > game->data.minPathLength) {
            free(surroundingTiles);
            free(path.tab);
            *pathValid = 1;
            return nextPath;
        }

        nextPath = findNextPath(game, nextPath, pathValid, startTime);

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
    if (game->data.maxPathLength < game->data.height - LAND_WATER_RATIO) {
        printf("Error: The maximum path length is too short for this terrain.\n");
        exit(1);
    }
    int maxPossibleLength = getMaxPathLength(game);
    if (game->data.minPathLength > maxPossibleLength) {
        printf("Error: The minimum length is too high for this terrain.\n");
        exit(1);
    }

    Path path;
    Path finalPath;
    int pathValid;

    constructPath(game, &path);
    path.tab[0] = findStart(game);
    path.length = 1;

    for (int i = 0; i < game->data.backoff.maxTries; i++) {
        pathValid = 0;
        finalPath = findNextPath(game, copyPath(game, path), &pathValid, time(NULL));
        if (pathValid && finalPath.length != 0) {
            break;
        }
        game->data.seed++;
        game->data.backoff.maxTime *= game->data.backoff.multiplier;
    }

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
    terrain[first_x][first_y] = START;

    for (i=1; i < path.length - 1; i++) {
        Coordinates pathCoor = path.tab[i];
        terrain[pathCoor.x][pathCoor.y] = PATH;
    }

    int last_x = path.tab[i].x;
    int last_y = path.tab[i].y;
    terrain[last_x][last_y] = CROWN;
}

Coordinates findDamageIndicatorCoordinates(Game* game, Coordinates objectCoord) {
    int success = 0;
    Coordinates* surroundingTiles = getSurroundingTiles(game, objectCoord, &success);
    for (int i = 0; i < success; i++) {
        if (!coordsInPath(surroundingTiles[i], game->path)) {
            Coordinates coord = surroundingTiles[i];
            free(surroundingTiles);
            return coord;
        }
    }

    // Will never get there
    free(surroundingTiles);
    Coordinates nullCoord;
    nullCoord.x = 0;
    nullCoord.y = 0;
    return nullCoord;
}

Crown constructCrown(Game* game) {
    Crown crown;
    crown.health = game->data.crownHealth;
    crown.damageIndicator.coord = findDamageIndicatorCoordinates(game, game->path.tab[game->path.length - 1]);
    crown.damageIndicator.nextTextFade = 0;
    crown.damageIndicator.nextColorFade = 0;
    return crown;
}

void updateCrown(Game* game) {
    if (game->crown.damageIndicator.nextTextFade > 0) {
        game->crown.damageIndicator.nextTextFade--;

        if (game->crown.damageIndicator.nextTextFade <= 0) {

            // Erase the textual damage indicator
            printTerrainTile(game, game->crown.damageIndicator.coord);
        }
    }

    if (game->crown.damageIndicator.nextColorFade > 0) {
        game->crown.damageIndicator.nextColorFade--;

        if (game->crown.damageIndicator.nextColorFade <= 0) {

            // Erase the color damage indicator
            printTerrainTile(game, game->path.tab[game->path.length - 1]);
        }
    }
}

void createTerrain(Game* game) {
    if (game->data.width * game->data.height < 3) {
        printf("Error: Terrain needs at least 3 tiles.\n");
        exit(1);
    }

    char** terrain = allocateTerrain(game->data.width, game->data.height);
    srand(game->data.seed);

    int x0 = game->data.width / 2;
    int y0 = game->data.height / 2;

    // Calculation of the horizontal and vertical rays of ellipse
    float ray1 = LAND_WATER_RATIO * game->data.width / 2;
    float ray2 = LAND_WATER_RATIO * game->data.height / 2;

    for (int x = 0; x < game->data.width; x++) {
        for (int y = 0; y < game->data.height; y++) {

            // Calculation of the equation of the ellipse with a random margin
            float ellipse = ((x - x0) * (x - x0)) / (ray1 * ray1) + ((y - y0) * (y - y0)) / (ray2 * ray2);
            float randomMargin = rand() % 1001 / 1000.0 * WATER_MAX_RANDOMNESS;

            float randomEmoji = (rand() % 10000) / 10000.0;

            if (ellipse + randomMargin < 1.0) {

                // Select a random land tile
                for (int i = 0; i < LAND_LAST - LAND_FIRST + 1; i++) {
                    if (randomEmoji >= LAND_PROBA[i]) {
                        terrain[x][y] = LAND_FIRST + i;
                        break;
                    }
                }

                // Generate a background land entity randomly
                for (BackgroundEntityType i = LAND_ENTITY_FIRST; i <= LAND_ENTITY_LAST; i++) {
                    if (randomEmoji < BACKGROUND_ENTITIES_PROBA[i]) {

                        // Set its coordinates
                        Coordinates entityCoord;
                        entityCoord.x = x;
                        entityCoord.y = y;

                        addBackgroundEntity(game, entityCoord, i);
                        break;
                    }
                }

            } else {
                // Add water
                terrain[x][y] = WATER;

                // Generate a water entity randomly
                for (BackgroundEntityType i = WATER_ENTITY_FIRST; i <= WATER_ENTITY_LAST; i++) {
                    if (randomEmoji < BACKGROUND_ENTITIES_PROBA[i]) {

                        // Set its coordinates
                        Coordinates entityCoord;
                        entityCoord.x = x;
                        entityCoord.y = y;

                        addBackgroundEntity(game, entityCoord, i);
                        break;
                    }
                }
            }
        }
    }
    game->terrain = terrain;
    game->path = generatePath(game);
    insertPath(terrain, game->path);
    game->crown = constructCrown(game);
    game->monkeySlots = generateMonkeySlots(game);
    insertMonkeySlots(game);
}




