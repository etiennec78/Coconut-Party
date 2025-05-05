#ifndef COMMON_H
#define COMMON_H

// MARK: - Constants
#define clear() printf("\033[H\033[2J") // NOTE: Clear screen
#define clearLine() printf("\x1b[2K\r") // NOTE: Clear line
#define color(param) printf("\033[%sm", param)

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
    Crown crown;
    Crabs crabs;
} Game;

// MARK: - Functions
void emptyBuffer();
void setRawMode(int enable);

#endif

/*
    0   Reset               1  Bold
    5   Flashing            7  reverse

                        Black   Red     Green   Yellow  Blue    magenta     cyan    White
    Characteres color   30      31      32      33      34      35          36      37 
    Background color    40      41      42      43      44      45          46      47 
*/