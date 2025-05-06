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
    int season;
    int minPathLength;
    int maxPathLength;
    int slotAmount;
    int crownHealth;
    Backoff backoff;
    int framerate;
    int refreshDelay;
    int soundEnabled;
} Data;



typedef struct {
    Coordinates coord;
    int nextTextFade; // In frames
    int nextColorFade; // In frames
} DamageIndicator;

typedef struct {
    int health;
    DamageIndicator damageIndicator;
} Crown;

typedef struct {
    float health;
    float defense;
    float speed; // In tiles per second
    float attack;
    float attackSpeed; // In attack per second
} CrabStats;

typedef struct {
    int dead;
    int pathIndex;
    Coordinates coord;
    CrabStats stats;
    int nextAttack; // In frames
    int nextPath; // In frames
} Crab;

typedef struct {
    Crab* tab;
    int length;
    int awaitingSpawn;
    int nextSpawn; // In frames
    int remaining;
} Crabs;

typedef struct {
    Data data;
    char** terrain;
    Path path;
    Data data;
    Path monkeySlots;
    Crown crown;
    Crabs crabs;
} Game;



#endif
