#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

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
    LAND_FIRST = 4,
    LAND_LAST = 9
} TerrainCase;

// MARK: - Constants
static const char* TERRAIN_TILES[][10] = {
    {"💧", "  ", "🏁", "👑", "🌿", "🌸", "🌺", "🌷", "🐝", "🐇"},
    {"🌊", "  ", "🏁", "👑", "🌳", "🌴", "🌻", "🌾", "🌱", "🦊"},
    {"💦", "  ", "🏁", "👑", "🍁", "🍂", "🌰", "🍄", "🐿️", "🦔"},
    {"🧊", "  ", "🏁", "👑", "🌲", "🪵", "🪾", "🌨️", "⛄", "🐻‍"}
};
static const unsigned char TERRAIN_TILE_COLORS[][10] = {
    {63, 175, 240, 220, 22, 22, 22, 22, 22, 22},
    {63, 94, 94, 226, 22, 22, 22, 22, 22, 22},
    {69, 172, 172, 220, 22, 22, 22, 22, 22 ,22},
    {111, 231, 231, 220, 22, 58, 22, 22, 22, 22}
};
static const char* ENTITIES[] = {"🦀"};

// MARK: - Structures

// MARK: - Declarations of functions
void printTerrain(Game* game);
void printTerrainTile(Game* game, Coordinates coord);
void printCrab(Game* game, Crab crab);
void moveEmojiCursor(Coordinates coord);
void resetColorBackground();
void hideCursor();
void showCursor();

#endif
