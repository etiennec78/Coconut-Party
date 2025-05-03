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
    int maxTime;
    int maxTries;
    int multiplier;
} Backoff;

typedef struct {
    int width;
    int height;
    int endHeight;
    unsigned int seed;
    int minPathLength;
    int maxPathLength;
    Backoff backoff;
} Data;

typedef struct {
    float health;
    float defense;
    float speed;
} CrabStats;

typedef struct {
    Coordinates coord;
    CrabStats stats;
} Crab;

typedef struct {
    Crab* tab;
    int length;
    int awaitingSpawn;
    int remaining;
} Crabs;

typedef struct {
    char** terrain;
    Path path;
    Data data;
    Crabs crabs;
} Game;

#endif
