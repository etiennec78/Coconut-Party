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
    int monkeyAmount;
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

typedef enum {
    NORMAL = 0,
    GIANT = 1,
    HEALER = 2,
    AGILE = 3,
    FLYING = 4,
    TANK = 5,
} CrabType;

typedef struct {
    float health;
    float defense;
    float speed; // In tiles per second
    int attack;
    float attackSpeed; // In attack per second
    int canFly;
    int canHeal;
} CrabStats;

typedef struct {
    CrabType type;
    int dead;
    int pathIndex;
    Coordinates coord;
    CrabStats stats;
    int nextAttack; // In frames
    int nextPath; // In frames
    DamageIndicator damageIndicator;
} Crab;

typedef struct {
    int attack;
    float attackSpeed;
    float attackDistance;
} MonkeyStats;

typedef enum {
    NOT_PLACED = 0,
    PLACED = 1,
} MonkeyType;

typedef struct {
    Crab* tab;
    int length;
    int awaitingSpawn;
    int nextSpawn; // In frames
    int remaining;
} Crabs;

typedef enum {
    LAND_ENTITY_FIRST = 0,
    LAND_ENTITY_LAST = 1,
    WATER_ENTITY_FIRST = 2,
    WATER_ENTITY_LAST = 5
} BackgroundEntityType;

typedef struct {
    BackgroundEntityType type;
    Coordinates coord;
    int nextMove;
} BackgroundEntity;

typedef struct {
    BackgroundEntity* tab;
    int length;
} BackgroundEntities;

typedef struct {
    int active;
    Coordinates coord;
    int nextCollection; // In frames
} Coin;

typedef struct {
    Coin* tab;
    int length;
} Coins;

typedef struct {
    MonkeyType type;
    Coordinates coord;
    MonkeyStats stats;
    int nextAttack; // In frames
} Monkey;

typedef struct {
    Monkey* tab;
    int length;
} Monkeys;

typedef struct {
    Data data;
    char** terrain;
    Path path;
    Crown crown;
    Crabs crabs;
    Monkeys monkeys;
    Coins coins;
    BackgroundEntities backgroundEntities;
} Game;

#endif
