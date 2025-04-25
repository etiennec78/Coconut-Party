#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

// MARK: - Constants
#define LAND_WATER_RATIO 0.9
#define WATER_MAX_RANDOMNESS 0.05

// MARK: - Structures

// MARK: - Declarations of functions
char** createTerrain(int width, int height);

#endif
