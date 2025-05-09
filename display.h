#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"
#include "backgroundEntities.h"

#define SCORE_COLUMN_WIDTH 12

typedef enum {
    SPRING = 0,
    SUMMER = 1,
    AUTUMN = 2,
    WINTER = 3
} Season;

typedef enum {
    WATER = 0,
    PATH = 1,
    START = 2,
    CROWN = 3,
    MONKEY_SLOT = 4,
    LAND_FIRST = 5,
    LAND_LAST = 9
} TerrainTile;

typedef enum {
    CRAB = 0,
    COIN = 1,
} EntityType;

typedef enum {
    UI_WAVE = 0,
    UI_COINS = 1,
    UI_KILLS = 2,
    UI_ALIVE = 3
} UIElement;

// MARK: - Constants
static const char* TERRAIN_TILES[][11] = {
    {"💧", "  ", "🏁", "👑", "🟪", "🌿", "🌸", "🌺", "🌷", "  "},
    {"🌊", "  ", "🏁", "👑", "🟫", "🌳", "🌴", "🌻", "🌾", "  "},
    {"💦", "  ", "🏁", "👑", "🟧", "🪵", "🍂", "🌰", "🍄", "  "},
    {"🧊", "  ", "🏁", "👑", "🟦", "🌲", "🪵", "🪵", "🎄", "  "}
};
static const unsigned char TERRAIN_TILE_COLORS[][11] = {
    {63, 175, 175, 220, 22, 22, 22, 22, 22, 223},
    {63, 94, 94, 226, 22, 22, 22, 28, 28, 222},
    {23, 94, 94, 220, 22, 58, 22, 22, 22, 137},
    {111, 255, 255, 220, 22, 22, 58, 58, 22, 75}
};

static const char* BACKGROUND_ENTITIES[][6] = {
    {"🐝", "🐇", "🐬", "🛥️", "🦐", "🐟"},
    {"🐿️", "🦊", "🦈", "🐙", "🚤", "🐠"},
    {"🦔", "🐗", "🦦", "🦑", "⛵", "🐡"},
    {"⛄", "🐻", "🐻‍❄️", "🐧", "🦭", "🐟"}
};

static const char* ENTITIES[] = {"🦀", "🪙"};
static const unsigned char CRAB_TYPE_COLORS[][6] = {
    {175, 136, 76, 220, 45, 244},
    {94, 136, 76, 220, 45, 244},
    {94, 136, 76, 220, 45, 244},
    {225, 136, 76, 220, 45, 244}
};

static const char* UI_ELEMENTS[4] = {
    "WAVE",
    "COINS",
    "KILLS",
    "ALIVE"
};
static const char* UI_EMOJIS[4] = {
    "🌊",
    "🪙",
    "💀",
    "🦀"
};

// MARK: - Structures

// MARK: - Declarations of functions
void printTerrain(Game* game);
void printTerrainTile(Game* game, Coordinates coord);
void refreshScores(Game* game);
void printScore(UIElement element, int data);
void refreshScores(Game* game);
void printCrab(Game* game, Crab crab);
void eraseCrab(Game* game, Crab crab);
void printCoin(Game* game, Coin coin);
void printCoinOnMap(Game* game, Coordinates coord);
void eraseCoin(Game* game, Coin coin);
void printDamage(Game* game, Coordinates coord, const char* tile, DamageIndicator indicator, int damage);
void printHeal(Game* game, Crab* crab, const char* tile, int heal);
void printBackgroundEntity(Game* game, BackgroundEntity entity);
void moveEmojiCursor(Coordinates coord);
void resetColorBackground();
void hideCursor();
void showCursor();

#endif
