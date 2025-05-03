#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

// MARK: - Constants
static const char* TERRAIN_CASES[][6] = {
    {"🌿", "🌸", "💧", "🟫", "🏁", "👑"},
    {"🌳", "🌴", "🌊", "🟨", "🏁", "👑"},
    {"🍁", "🌱", "💦", "🟫", "🏁", "👑"},
    {"🌲", "🪵", "🧊", "⬜", "🏁", "👑"}
};
static const char* ENTITIES[] = {"🦀"};

// MARK: - Structures

// MARK: - Declarations of functions
void printTerrain(Game* game);
void printTerrainTile(Game* game, Coordinates coord);
void printCrabs(Game* game);
void moveEmojiCursor(Coordinates coord);
void hideCursor();
void showCursor();

#endif
