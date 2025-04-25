#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

// MARK: - Constants
#define LAND_WATER_RATIO 0.9
#define WATER_MAX_RANDOMNESS 0.25

// MARK: - Structures

// MARK: - Declarations of functions
void createTerrain(Game* game);
void freeTerrain(char** terrain);

#endif
