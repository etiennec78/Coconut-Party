#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "common.h"
#include "asciiArt.h"
#include "menus.h"
#include "terrain.h"

// MARK: - Manipulation of items values
void setItemValue(Game* game, int* item, char* itemValue, const char* seasonItems[]) {
    switch(*item) {
        case 0:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.width);
            break;
        case 1:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.height);
            break;
        case 2:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %u >", game->data.seed);
            break;
        case 3:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %s >", seasonItems[game->data.season]);
            break;
        case 4:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.minPathLength);
            break;
        case 5:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.maxPathLength);
            break;
        case 6:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.crownHealth);
            break;
        case 7:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.framerate);
            break;
        case 8:
            if(game->data.soundEnabled) {
                snprintf(itemValue, ITEM_VALUE_LEN, "< On >");
            } else {
                snprintf(itemValue, ITEM_VALUE_LEN, "< Off >");
            }
            break;
        case 9:
        case 10:
            break;
        default:
            printf("🚨 An error occurred while displaying option values ! (#%d)\n", *item);
            exit(1);
            break;
    }
}
void updateGameData(Game* game, int* item, int incr) {
    switch(*item) {
        case 0:
            if(game->data.width < WIDTH_MAX && incr == 1) {
                game->data.width++;
            } else if(game->data.width > WIDTH_MIN &&incr == -1) {
                game->data.width--;
            }
            break;
        case 1:
            if(game->data.height < HEIGHT_MAX && incr == 1) {
                game->data.height++;
            } else if(game->data.height > HEIGHT_MIN &&incr == -1) {
                game->data.height--;
            }
            break;
        case 2:
            if(game->data.seed < UINT_MAX && incr == 1) { // NOTE: Valeur max de unsigned int
                game->data.seed++;
            } else if(game->data.seed > 0 &&incr == -1) {
                game->data.seed--;
            }
            break;
        case 3:
            if(game->data.season < 3 && incr == 1) {
                game->data.season++;
            } else if(game->data.season > 0 &&incr == -1) {
                game->data.season--;
            }
            break;
        case 4:
            if(game->data.minPathLength < game->data.maxPathLength-1 && incr == 1) {
                game->data.minPathLength++;
            } else if(game->data.minPathLength > (game->data.height - LAND_WATER_RATIO) && incr == -1) {
                game->data.minPathLength--;
            }
            break;
        case 5:
            if(game->data.maxPathLength < getMaxPathLength(game) && incr == 1) {
                game->data.maxPathLength++;
            } else if(game->data.maxPathLength > game->data.minPathLength+1 && incr == -1) {
                game->data.maxPathLength--;
            }
            break;
        case 6:
            if(game->data.crownHealth < UINT_MAX && incr == 1) {
                game->data.crownHealth++;
            } else if(game->data.crownHealth > 0 && incr == -1) {
                game->data.crownHealth--;
            }
            break;
        case 7:
            if(game->data.framerate < FRAMERATE_MAX && incr == 1) {
                game->data.framerate++;
            } else if(game->data.framerate > FRAMERATE_MIN && incr == -1) {
                game->data.framerate--;
            }
            break;
        case 8:
            if(game->data.soundEnabled && incr == 1) {
                game->data.soundEnabled = 0;
            } else if(!game->data.soundEnabled && incr == -1) {
                game->data.soundEnabled = 1;
            }
            break;
        case 9:
        case 10:
            break;
        default:
            printf("🚨 An error occurred during the update of game datas ! (#%d)\n", *item);
            exit(1);
            break;
    }
}

// MARK: - Main menu
void displayMainMenu(const char* mainItems[], int* activeItem, int* itemsWidth, char* pressedKey) {    
    // NOTE: List and shape menu items
    for(int i=0; i<MAIN_ITEMS; i++) {
        for(int s=0; s<=(114-(*itemsWidth))/(MAIN_ITEMS+1); s++) {
            printf(" ");
        }
        if(i == *activeItem) {
            color("7");
            printf("* ");
        }
        printf("%s", mainItems[i]);
        color("0");
    }
    
    // NOTE: Detect keyboard key
    *pressedKey = getchar();
    if(*pressedKey == '\033') {
        if((*pressedKey = getchar()) == '[') { // NOTE: Skip [ char
            switch(getchar()) {
                case 'C': // NOTE: Arrow right
                    if(*activeItem < MAIN_ITEMS-1) {
                        (*activeItem)++;
                    }
                    break;
                case 'D': // NOTE: Arrow left
                    if(*activeItem > 0) {
                        (*activeItem)--;
                    }
                    break;
            }

            clearLine();
            displayMainMenu(mainItems, activeItem, itemsWidth, pressedKey);
        } else {
            *activeItem = MAIN_ITEMS-1;
        }
    } else if(*pressedKey != '\r' && *pressedKey != '\n') {
        clearLine();
        displayMainMenu(mainItems, activeItem, itemsWidth, pressedKey);
    }
}
void mainMenu(int* activeItem) {
    int *itemsWidth = malloc(sizeof(int));
    char *pressedKey = malloc(5 * sizeof(char));

    if(itemsWidth == NULL) {
        printf("🚨 Memory allocation failed for itemsWidth !\n");
        exit(1);
    }
    if(pressedKey == NULL) {
        printf("🚨 Memory allocation failed for pressedKey (main menu) !\n");
        exit(1);
    }

    // NOTE: Calcul total caract
    *itemsWidth = 0;
    for(int iw=0; iw<MAIN_ITEMS; iw++) {
        *itemsWidth += strlen(mainItems[iw]);
    }

    clear();
    asciiArt("coconutParty");
    setRawMode(1); // NOTE: Enable row mode

    displayMainMenu(mainItems, activeItem, itemsWidth, pressedKey);

    setRawMode(0); // NOTE: Restore canonique mode
    free(itemsWidth);
    free(pressedKey);
}

// MARK: - Options menu
void displayOptionsMenu(Game* game, const char* optionsItems[], Options* items, int numberOfItems, char* itemValue, int* activeItem, int* itemUsed, const char* seasonItems[], char* pressedKey) {
    // NOTE: List and shape menu items
    for(int i=0; i<numberOfItems; i++) {
        *itemUsed = (items == NULL) ? i : items[i]; // NOTE: {condition} ? {action if condition is verified} : {action if condition isn't verified}

        setItemValue(game, itemUsed, itemValue, seasonItems); // NOTE: Set value format to diaply for item

        if(i == *activeItem) {
            color("7");
        }

        printf("%s", optionsItems[*itemUsed]);
        if(*itemUsed != START_CUSTOM_GAME && *itemUsed != BACK) {
            for(int s=0; s<=68-strlen(optionsItems[*itemUsed])-strlen(itemValue); s++) { // NOTE: Set number of space for "space between" format
                printf(" ");
            }
            printf("%s\n", itemValue);
        } else {
            printf("\n");
        }

        color("0");
    }

    // NOTE: activeItem = index of item selected in menu in list of items displayed, itemUsed = unique index of active item (index in "Options" enum)
    *itemUsed = (items == NULL) ? *activeItem : items[*activeItem]; // NOTE: If items is NULL, use items index else use index defined in Options enum

    // NOTE: Detect keyboard key
    *pressedKey = getchar();
    if(*pressedKey == '\033') {
        getchar(); // NOTE: Skip [ char
        switch(getchar()) {
            case 'A': // NOTE: Arrow top
                if(*activeItem > 0) {
                    (*activeItem)--;
                }
                break;
            case 'B': // NOTE: Arrow down
                if(*activeItem < numberOfItems-1) {
                    (*activeItem)++;
                }
                break;
            case 'C': // NOTE: Arrow right
                updateGameData(game, itemUsed, 1);
                break;
            case 'D': // NOTE: Arrow left
                updateGameData(game, itemUsed, -1);
                break;
        }

        // NOTE: Clear options block
        clearLine();
        prevLine(numberOfItems);
        displayOptionsMenu(game, optionsItems, items, numberOfItems, itemValue, activeItem, itemUsed, seasonItems, pressedKey);
    } else if(*itemUsed != START_CUSTOM_GAME && *itemUsed != BACK && (*pressedKey != '\r' || *pressedKey != '\n')) {
        // NOTE: Clear options block
        clearLine();
        prevLine(numberOfItems);
        displayOptionsMenu(game, optionsItems, items, numberOfItems, itemValue, activeItem, itemUsed, seasonItems, pressedKey);
    }

    *activeItem = *itemUsed;
    if(*itemUsed == BACK) {
        initGameDatas(game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGHT, DEFAULT_MAX_PATH_LENGHT, 1);
    }
}
void optionsMenu(const char* title, Game* game, Options* items, int numberOfItems, int* activeItem) {
    int *itemUsed = malloc(sizeof(int));
    char *pressedKey = malloc(5 * sizeof(char)), *itemValue = malloc(ITEM_VALUE_LEN * sizeof(char));

    if(itemUsed == NULL) {
        printf("🚨 Memory allocation failed for itemUsed !\n");
        exit(1);
    }
    if(pressedKey == NULL) {
        printf("🚨 Memory allocation failed for pressedKey (options menu) !\n");
        exit(1);
    }
    if(itemValue == NULL) {
        printf("🚨 Memory allocation failed for itemValue !\n");
        exit(1);
    }

    clear();
    asciiArt(title);
    setRawMode(1); // NOTE: Enable row mode

    displayOptionsMenu(game, optionsItems, items, numberOfItems, itemValue, activeItem, itemUsed, seasonItems, pressedKey);
 
    setRawMode(0); // NOTE: Restore canonique mode
    free(pressedKey);
    free(itemValue);
}