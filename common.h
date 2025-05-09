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
    float defaultHealth;
    float defense;
    int attack;
    float defaultSpeed; // In tiles per second
    float speed; // In tiles per second
    float defaultAttackSpeed; // In attack per second
    float attackSpeed; // In attack per second
    int canFly;
    int canHeal;
    int heal;
    int healSpeed;
} CrabStats;

typedef struct {
    CrabType type;
    int dead;
    int pathIndex;
    Coordinates coord;
    CrabStats stats;
    int nextAttack; // In frames
    int nextPath; // In frames
    int nextUnfreeze; // In frames
    int nextHeal; // In frames
    DamageIndicator damageIndicator;
} Crab;

typedef struct {
    int attack;
    float attackSpeed;
    float attackDistance;
    int canFreeze;
} MonkeyStats;

typedef enum {
    NOT_PLACED = 0,
    ALPHA = 1,
    BALLISTIC = 2,
    PALMSHAKER = 3,
    HYPERACTIVE = 4,
    STUNNER = 5,
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

typedef enum {
    COIN_DISABLED,
    COIN_ON_MAP,
    COIN_COLLECTION
} CoinState;

typedef struct {
    CoinState state;
    Coordinates coord;
    Coordinates startCoord;
    int nextCollection; // In frames
    double collectionProgression;
} Coin;

typedef struct {
    Coin* tab;
    int length;
    Coordinates scoreCoord;
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
    int wave;
    int coins;
    int kills;
    int remainingCrabs;
} Score;

typedef struct {
    Data data;
    Score score;
    char** terrain;
    Path path;
    Crown crown;
    Crabs crabs;
    Monkeys monkeys;
    Coins coins;
    BackgroundEntities backgroundEntities;
} Game;

#endif
