#ifndef COMMON_H
#define COMMON_H

// MARK: - Importations
#include <time.h>

// MARK: - Constants
#define DEFAULT_WIDTH 40
#define DEFAULT_HEIGHT 30
#define DEFAULT_SEED time(NULL)
#define DEFAULT_MIN_PATH_LENGTH 30
#define DEFAULT_MAX_PATH_LENGTH 200
#define DEFAULT_CROWN_HEALTH 100

#define WIDTH_MAX 130
#define WIDTH_MIN 10
#define HEIGHT_MAX 80
#define HEIGHT_MIN 10
#define FRAMERATE_MAX 144
#define FRAMERATE_MIN 1

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
    int id;
    Data data;
    char** terrain;
    Path path;
    Crown crown;
    Crabs crabs;
} Game;

// MARK: - Functions
void initGameData(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength, int crownHealth, int isMenu);
void emptyBuffer();
void setRawMode(int enable);

#endif
