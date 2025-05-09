#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

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
int coordsEqual(Coordinates coord1, Coordinates coord2);
int coordsInPath(Coordinates coord, Path path);
int coordsInTerrain(Game* game, Coordinates coord);
Coordinates terrainCoordToGlobalCoord(Coordinates coord);
int getIndexAtCoordinates(Path path, Coordinates coord);
Coordinates* getSurroundingTiles(Game* game, Coordinates currentCoordinates, int* surroundingLength);
void updateCrown(Game* game);

#endif
