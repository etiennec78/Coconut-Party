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
#define DEFAULT_MONKEY_AMOUNT 15
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
    int monkeyAmount;
    int crownHealth;
    Backoff backoff;
    int framerate;
    int refreshDelay;
    int soundEnabled;
} Data;

typedef struct {
    int seed;
    int width;
    int height;
} LevelData;

typedef enum {
    LVL_1 = 1, LVL_2, LVL_3, LVL_4, LVL_5,
    LVL_6, LVL_7, LVL_8, LVL_9, LVL_10,
    LVL_11, LVL_12, LVL_13, LVL_14, LVL_15,
    LVL_16, LVL_17, LVL_18, LVL_19, LVL_20, LVL_ABOVE20,
} Level;

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
    int currentLevel;
} Score;

typedef struct {
    int id;
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

// MARK: - Functions
void initGameData(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength, int monkeyAmount, int crownHealth, int isMenu);
void emptyBuffer();
void setRawMode(int enable);

#endif
