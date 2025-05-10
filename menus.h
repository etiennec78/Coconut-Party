#ifndef MENUS_H
#define MENUS_H

// MARK: - Constants
#define ITEM_VALUE_LEN 15

#define MAIN_ITEMS 5
#define CUSTOM_GAME_ITEMS 9
#define RESTORE_GAME_ITEMS 2
#define OPTIONS_ITEMS 3
#define PAUSE_ITEMS 4

static const char* noGameSavedMessage = "No game saved ! Press backspace for return to the main menu";
static const char* menuItems[] = {
    // NOTE: Game settings
    "Map with",
    "Map height",
    "Generation seed",
    "Season",
    "Minimum path length",
    "Maximum path length",
    "Crown health",
    "Frame rate",
    "Sound",

    // NOTE: Main menus
    "New game",
    "Custom game",
    "Restore game",
    "Options",
    "Exit",

    // NOTE: Game actions
    "Start game",
    "Resume game",
    "Save & quit",
    "\033[31mQuit game\033[0m", // NOTE: Set color to red

    // NOTE: Common items
    '\0', // NOTE: String list is not a button
    "Back",
};

static const char* seasonItems[] = {
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
    // NOTE: Game settings
    MAP_WIDTH = 0,
    MAP_HEIGHT = 1,
    SEED = 2,
    SEASON = 3,
    MIN_PATH_LENGHT = 4,
    MAX_PATH_LENGHT = 5,
    CROWN_HEALTH = 6,
    FRAME_RATE = 7,
    SOUND = 8,

    // NOTE: Main menus
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
    STRING_LIST = 18,
    BACK = 19
} MenuItem;

// MARK: - Functions
void setItemValue(Game* game, MenuItem item, char* itemValue);
void updateGameData(Game* game, MenuItem item, int incr);

void displayHorizontalMenu(Game* game, MenuItem* items, int numberOfItems, int* itemsWidth, int* activeItem, int itemMarker, char* pressedKey);
void displayVerticalMenu(Game* game, MenuItem* items, char** stringItems, int numberOfItems, char* itemValue, int* activeItem, int itemMarker, char* pressedKey);
void menu(char* title, MenuDirection direction, Game* game, MenuItem* items, char** stringItems, int numberOfItems, int* selectedItem, int itemMarker);

void mainMenu(Game* game, int* selectedItem);
void customGameMenu(Game* game, int* selectedItem);
int restoreGameMenu(Game* game, int* selectedItem);
void optionsMenu(Game* game, int* selectedItem);
void pauseMenu(Game* game);

// void displayRestoreGameMenu(char** gamesNameList, int countGameName, int* activeItem, char* pressedKey);

#endif