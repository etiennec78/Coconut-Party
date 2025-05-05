#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

#define TREE1 0
#define TREE2 1
#define WATER 2

// MARK: - Constants
#define LAND_WATER_RATIO 0.9
#define WATER_MAX_RANDOMNESS 0.2
#define FINISH_LINE_RATIO 0.1


// MARK: - Structures

// MARK: - Declarations of functions
void createTerrain(Game* game);
void freeTerrain(char** terrain);
void constructPath(Game* game, Path* path);
void shuffleCoords(Coordinates* list, int N);
Coordinates* getSurroundingTiles(Game* game, Coordinates currentCoordinates, int* surroundingLength);
int coordsInPath(Coordinates coord, Path path);

#endif

