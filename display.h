#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"
#include "backgroundEntities.h"

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
    LAND_LAST = 8
} TerrainTile;

typedef enum {
    CRAB = 0,
    COIN = 1,
} EntityType;

// MARK: - Constants
static const char* TERRAIN_TILES[][9] = {
    {"💧", "  ", "🏁", "👑", "🟪", "🌿", "🌸", "🌺", "🌷"},
    {"🌊", "  ", "🏁", "👑", "🟫", "🌳", "🌴", "🌻", "🌾"},
    {"💦", "  ", "🏁", "👑", "🟧", "🌰", "🍂", "🌰", "🍄"},
    {"🧊", "  ", "🏁", "👑", "🟦", "🌲", "🪵", "🪵", "🎄"}
};
static const unsigned char TERRAIN_TILE_COLORS[][11] = {
    {63, 175, 175, 220, 22, 22, 22, 22, 22},
    {63, 94, 94, 226, 22, 22, 22, 28, 28},
    {23, 94, 94, 220, 22, 58, 22, 22, 22},
    {111, 255, 255, 220, 22, 22, 58, 58, 22}
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

// MARK: - Structures

// MARK: - Declarations of functions
void printTerrain(Game* game);
void printTerrainTile(Game* game, Coordinates coord);
void printCrab(Game* game, Crab crab);
void eraseCrab(Game* game, Crab crab);
void printCoin(Game* game, Coordinates coord);
void eraseCoin(Game* game, Coin coin);
void printDamage(Game* game, Coordinates coord, const char* tile, DamageIndicator indicator, int damage);
void printBackgroundEntity(Game* game, BackgroundEntity entity);
void moveEmojiCursor(Coordinates coord);
void resetColorBackground();
void hideCursor();
void showCursor();

#endif
