#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

// MARK: - Constants
static const char* SEASONS[][7] = {
    {"🌿", "🌸", "💧", "🟫", "🏁", "👑", "🐒"},
    {"🌳", "🌴", "🌊", "🟨", "🏁", "👑", "🐒"},
    {"🍁", "🌱", "💦", "🟫", "🏁", "👑", "🐒"},
    {"🌲", "🪵", "🧊", "⬜", "🏁", "👑", "🐒"}
};

// MARK: - Structures

// MARK: - Declarations of functions
void printGame(Game* game);

#endif
