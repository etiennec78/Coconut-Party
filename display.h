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
    TREE1 = 0,
    TREE2 = 1,
    WATER = 2,
    PATH = 3,
    START = 4,
    CROWN = 5
} TerrainCase;

// MARK: - Constants
static const char* TERRAIN_CASES[][6] = {
    {"🌿", "🌸", "💧", "  ", "🏁", "👑"},
    {"🌳", "🌴", "🌊", "  ", "🏁", "👑"},
    {"🍁", "🌱", "💦", "  ", "🏁", "👑"},
    {"🌲", "🪵", "🧊", "  ", "🏁", "👑"}
};
static const unsigned char TERRAIN_CASE_COLORS[][6] = {
    {22, 22, 63, 175, 240, 220},
    {22, 22, 63, 94, 94, 226},
    {22, 22, 69, 172, 172, 220},
    {22, 58, 111, 231, 231, 220}
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
