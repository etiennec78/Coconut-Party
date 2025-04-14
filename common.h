#ifndef COMMON_H
#define COMMON_H

// MARK: - Constants

// MARK: - Structures
typedef struct{
    int x;
    int y;
} Coordinates;

typedef struct {
    int width;
    int height;
} Data;

typedef struct {
    char** terrain;
    Data data;
} Game;

#endif
