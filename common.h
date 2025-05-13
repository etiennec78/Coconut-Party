#ifndef COMMON_H
#define COMMON_H

// MARK: - Importations
#include <time.h>
#include <limits.h>

// MARK: - Constants
#define DEFAULT_WIDTH 40
#define DEFAULT_HEIGHT 30
#define DEFAULT_SEED time(NULL)
#define DEFAULT_MIN_PATH_LENGTH 30
#define DEFAULT_MAX_PATH_LENGTH 200
#define DEFAULT_MONKEY_AMOUNT 15
#define DEFAULT_CROWN_HEALTH 100

#define GAME_NAME_LEN_MAX 31
#define WIDTH_MAX 130
#define WIDTH_MIN 10
#define HEIGHT_MAX 80
#define HEIGHT_MIN 10
#define FRAMERATE_MAX 144
#define FRAMERATE_MIN 1
#define MAX_COINS 101

extern const int MONKEY_PRICES[];

// MARK: - Structures
typedef struct {
    int id;
    char name[GAME_NAME_LEN_MAX];
    int isAlreadySaved;
} Header;

typedef struct {
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
    int endWidth;
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
    int destroyed;
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
    int price;
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
    float nextWave; // In seconds
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

typedef enum {
    SHOP_NONE = -1,
    SHOP_WAVE = 0,
    SHOP_TYPE = 1,
    SHOP_MONKEY = 2,
    SHOP_BUY = 3
} MonkeyShopMenu;

typedef struct {
    MonkeyShopMenu focusedMenu;
    int selectedMonkey;
    MonkeyType selectedType;
} MonkeyShop;

typedef struct {
    Monkey* tab;
    int length;
    MonkeyShop shop;
} Monkeys;

typedef struct {
    int wave;
    int coins;
    int kills;
    int remainingCrabs;
} Score;

typedef struct {
    int nextMonkeyPop;
    int poppedIndex;
} EndAnimation;

typedef struct {
    Header header;
    Data data;
    Score score;
    char** terrain;
    Path path;
    Crown crown;
    Crabs crabs;
    Monkeys monkeys;
    Coins coins;
    BackgroundEntities backgroundEntities;
    EndAnimation end;
} Game;

// MARK: - Functions
void initGameData(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength, int monkeyAmount, int crownHealth, int isMenu);
void emptyBuffer();
void setRawMode(int enable);

#endif
