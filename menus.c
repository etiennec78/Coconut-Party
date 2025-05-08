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
            printf("🚨 An error occurred while displaying option values ! (#%d)\n", *item); // FIXEHERE: (#89700699)
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
void displayMainMenu(const char* mainItems[], int* activeItem) {
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

    // Calcul de la largeur totale occupée par les mots
    *itemsWidth = 0;
    for(int iw=0; iw<MAIN_ITEMS; iw++) {
        *itemsWidth += strlen(mainItems[iw]);
    }
    
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
            displayMainMenu(mainItems, activeItem);
        } else {
            *activeItem = MAIN_ITEMS-1;
        }
    } else if(*pressedKey != '\r' && *pressedKey != '\n') {
        clearLine();
        displayMainMenu(mainItems, activeItem);
    }
}
int mainMenu() {
    int activeItem = 0;

    clear();
    asciiArt("coconutParty");
    setRawMode(1); // NOTE: Enable row mode

    displayMainMenu(mainItems, &activeItem);

    setRawMode(0); // NOTE: Restore canonique mode

    return activeItem;
}

// MARK: - Options menu
void displayOptionsMenu(Game* game, const char* optionsItems[], Options* items, int* activeItem, const char* seasonItems[]) {
    int *numberOfItems = malloc(sizeof(int)), *item = malloc(sizeof(int));
    char *pressedKey = malloc(5 * sizeof(char)), *itemValue = malloc(ITEM_VALUE_LEN * sizeof(char));

    if(numberOfItems == NULL) {
        printf("🚨 Memory allocation failed for numberOfItems !\n");
        exit(1);
    }
    if(item == NULL) {
        printf("🚨 Memory allocation failed for item !\n");
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

    *numberOfItems = (items == NULL) ? OPTIONS_ITEMS-1 : countItems(items);
    
    // NOTE: List and shape menu items
    for(int i=0; i<=*numberOfItems; i++) {
        *item = (items == NULL) ? i : items[i];

        setItemValue(game, item, itemValue, seasonItems); // NOTE: Set value format to diaply for item

        if(i == *activeItem) {
            color("7");
        }

        printf("%s", optionsItems[*item]);
        if(*item < 9) {
            for(int s=0; s<=68-strlen(optionsItems[*item])-strlen(itemValue); s++) { // NOTE: Set number of space for shape a "space between"
                printf(" ");
            }
            printf("%s\n", itemValue);
        } else if(*item < *numberOfItems) {
            printf("\n");
        }

        color("0");
    }

    *item = (items == NULL) ? *activeItem : items[*activeItem];

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
                if(*activeItem < *numberOfItems) {
                    (*activeItem)++;
                }
                break;
            case 'C': // NOTE: Arrow right
                updateGameData(game, item, 1);
                break;
            case 'D': // NOTE: Arrow left
                updateGameData(game, item, -1);
                break;
        }

        // NOTE: Clear options block
        clearLine();
        prevLine(*numberOfItems);

        free(numberOfItems);
        free(item);
        free(pressedKey);
        free(itemValue);

        displayOptionsMenu(game, optionsItems, items, activeItem, seasonItems);
    } else if(*item < 9 && (*pressedKey != '\r' || *pressedKey != '\n')) {
        // NOTE: Clear options block
        clearLine();
        prevLine(*numberOfItems);

        free(numberOfItems);
        free(item);
        free(pressedKey);
        free(itemValue);

        displayOptionsMenu(game, optionsItems, items, activeItem, seasonItems);
    }
    
    if(items == NULL && *item == BACK) {
        initGameDatas(game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGHT, DEFAULT_MAX_PATH_LENGHT);
    }
}
int optionsMenu(const char* title, Game* game, Options* items) {
    int activeItem = 0;

    clear();
    asciiArt(title);
    setRawMode(1); // NOTE: Enable row mode

    displayOptionsMenu(game, optionsItems, items, &activeItem, seasonItems);
 
    setRawMode(0); // NOTE: Restore canonique mode

    return activeItem;
}