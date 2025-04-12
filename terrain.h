#ifndef TERRAIN_H
#define TERRAIN_H

// MARK: - Constants

// MARK: - Structures
typedef struct{
    int x;
    int y;
} Coordinates;

// MARK: - Déclarations of functions
char** allocateTerrain(int width, int height);
char** createTerrain(int width, int height);

#endif