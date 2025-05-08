#ifndef MENUS_H
#define MENUS_H

// MARK: - Constants
#define ITEM_VALUE_LEN 15

#define MAIN_ITEMS 5
#define OPTIONS_ITEMS 11

static const char* mainItems[] = { "New game", "Custom game", "Restore game" , "Options", "Exit" };
static const char* optionsItems[] = {
    "Map with",
    "Map height",
    "Generation seed",
    "Season",
    "Minimum path length",
    "Maximum path length",
    "Crown health",
    "Frame rate" ,
    "Sound",
    "Start custom game",
    "Back"
};
static const char* seasonItems[] = {
    "Spring",
    "Summer",
    "Autumn",
    "Winter"
};

// MARK: - Structures
typedef enum {
    MAP_WIDTH = 0,
    MAP_HEIGHT = 1,
    SEED = 2,
    SEASON = 3,
    MIN_PATH_LENGHT = 4,
    MAX_PATH_LENGHT = 5,
    CROWN_HEALTH = 6,
    FRAME_RATE = 7,
    SOUND = 8,
    START_CUSTOM_GAME = 9,
    BACK = 10
} Options;

// MARK: - Functions
void setItemValue(Game* game, int* item, char* itemValue, const char* seasonItems[]);
void updateGameData(Game* game, int* item, int incr);

void displayMainMenu(const char* mainItems[], int* activeItem, int* itemsWidth, char* pressedKey);
void mainMenu(int * activeItem);

void displayOptionsMenu(Game* game, const char* optionsItems[], Options* items, int numberOfItems, char* itemValue, int* activeItem, int* itemUsed, const char* seasonItems[], char* pressedKey);
void optionsMenu(const char* title, Game* game, Options* items, int numberOfItems, int* activeItem);

#endif