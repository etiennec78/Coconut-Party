#ifndef COMMON_H
#define COMMON_H

// MARK: - Constants

// MARK: - Structures
typedef struct{
    int x;
    int y;
} Coordinates;

typedef struct {
    Coordinates* tab;
    int length;
} Path;

typedef struct {
    int width;
    int height;
    int endHeight;
    unsigned int seed;
    int minPathLength;
    int maxPathLength;
} Data;

typedef struct {
    char** terrain;
    Path path;
    Data data;
} Game;

#endif
