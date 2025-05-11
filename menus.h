#ifndef MENUS_H
#define MENUS_H

#include "common.h"

// MARK: - Constants
#define ITEM_VALUE_LEN 15

#define MAIN_ITEMS 5
#define CUSTOM_GAME_ITEMS 9
#define RESTORE_GAME_ITEMS 2
#define OPTIONS_ITEMS 3
#define PAUSE_ITEMS 4

static const char* MENU_ITEMS[] = {
    // NOTE: Custom game settings
    "Map with",
    "Map height",
    "Generation seed",
    "Season",
    "Minimum path length",
    "Maximum path length",
    "Crown health",
    "Frame rate",
    "Sound",

    // NOTE: Main menu
    "New game",
    "Custom game",
    "Restore game",
    "Options",
    "Exit",

    // NOTE: Game actions
    "Start game",
    "Resume game",
    "Save & quit",
    "\033[31mQuit game\033[0m", // Set color to red

    // NOTE: Common items
    "\0", // NOTE: String list is not a button
    "Back",
};

static const char* SEASON_ITEMS[] = {
    "Spring",
    "Summer",
    "Autumn",
    "Winter"
};

// MARK: - Structures
typedef enum {
    HORIZONTAL_MENU = 0,
    VERTICAL_MENU = 1
} MenuDirection;

typedef enum {
    // NOTE: Custom game settings
    MAP_WIDTH = 0,
    MAP_HEIGHT = 1,
    SEED = 2,
    SEASON = 3,
    MIN_PATH_LENGTH = 4,
    MAX_PATH_LENGTH = 5,
    CROWN_HEALTH = 6,
    FRAME_RATE = 7,
    SOUND = 8,

    // NOTE: Main menu
    NEW_GAME = 9,
    CUSTOM_GAME = 10,
    RESTORE_GAME = 11,
    OPTIONS = 12,
    EXIT = 13,

    // NOTE: Game actions
    START_GAME = 14,
    RESUME_GAME = 15,
    SAVE_QUIT = 16,
    QUIT_GAME = 17,

    // NOTE: Common items
    STRING_LIST = 19,
    BACK = 20
} MenuItem;

// MARK: - Functions
void setItemValue(Game* game, MenuItem item, char* itemValue);
void updateGameData(Game* game, MenuItem item, int incr);

void displayHorizontalMenu(Game* game, MenuItem* items, int numberOfItems, int itemsWidth, MenuItem* activeItem, int itemMarker, char* pressedKey);
void displayVerticalMenu(Game* game, MenuItem* items, char** stringItems, int numberOfItems, char* itemValue, MenuItem* activeItem, int itemMarker, char* pressedKey);
void menu(char* title, MenuDirection direction, Game* game, MenuItem* items, char** stringItems, int numberOfItems, MenuItem* selectedItem, int itemMarker);

void mainMenu(Game* game, MenuItem* selectedItem);
void customGameMenu(Game* game, MenuItem* selectedItem);
int restoreGameMenu(Game* game, MenuItem* selectedItem);
void optionsMenu(Game* game, MenuItem* selectedItem);
void listenToKeyboard(Game* game);

#endif
