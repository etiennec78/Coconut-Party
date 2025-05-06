#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

// MARK: - Constants
#define LAND_WATER_RATIO 0.9
#define WATER_MAX_RANDOMNESS 0.2
#define FINISH_LINE_RATIO 0.1

const float LAND_PROBA[6] = {
    // 35%, 35%, 20%, 10%
    0.65, 0.30, 0.1, 0
};

const float BACKGROUND_ENTITIES_PROBA[6] = {
    // Land: 1%, 2%
    // Water: 0.1%, 0.1%, 0.3%, 0.5%
    0.001, 0.003, 0.001, 0.002, 0.005, 0.01
};

// MARK: - Structures

// MARK: - Declarations of functions
void createTerrain(Game* game);
void freeTerrain(char** terrain);
void constructPath(Game* game, Path* path);
void shuffleCoords(Coordinates* list, int N);
int coordsEqual(Coordinates coord1, Coordinates coord2);
int coordsInPath(Coordinates coord, Path path);
int getIndexAtCoordinates(Path path, Coordinates coord);
Coordinates* getSurroundingTiles(Game* game, Coordinates currentCoordinates, int* surroundingLength);
void updateCrown(Game* game);

#endif

