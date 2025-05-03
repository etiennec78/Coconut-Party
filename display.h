#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

// MARK: - Constants
static const char* SEASONS[][6] = {
    {"🌿", "🌸", "💧", "🟫", "🏁", "👑"},
    {"🌳", "🌴", "🌊", "🟨", "🏁", "👑"},
    {"🍁", "🌱", "💦", "🟫", "🏁", "👑"},
    {"🌲", "🪵", "🧊", "⬜", "🏁", "👑"}
};
static const char* ENTITIES[] = {"🦀"};

// MARK: - Structures

// MARK: - Declarations of functions
void printTerrain(char** terrain, int width, int height);
void printTerrainTile(Game* game, Coordinates coord);
void printCrabs(Game* game);
void moveEmojiCursor(Coordinates coord);

#endif
