#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/select.h>

#include "common.h"
#include "asciiArt.h"
#include "display.h"
#include "menus.h"
#include "terrain.h"
#include "monkeys.h"

// MARK: - Manipulation of items values
void setItemValue(Game* game, MenuItem item, char* itemValue) { // NOTE: Set value format to diaply for item
    switch(item) {
        // NOTE: Game settings
        case MAP_WIDTH:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.width);
            break;
        case MAP_HEIGHT:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.height);
            break;
        case SEED:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %u >", game->data.seed);
            break;
        case SEASON:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %s >", SEASON_ITEMS[game->data.season]);
            break;
        case MIN_PATH_LENGTH:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.minPathLength);
            break;
        case MAX_PATH_LENGTH:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.maxPathLength);
            break;
        case CROWN_HEALTH:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.crownHealth);
            break;
        case FRAME_RATE:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.framerate);
            break;
        case SOUND:
            if (game->data.soundEnabled) {
                snprintf(itemValue, ITEM_VALUE_LEN, "< On >");
            } else {
                snprintf(itemValue, ITEM_VALUE_LEN, "< Off >");
            }
            break;
        case WAVE:
            snprintf(itemValue, ITEM_VALUE_LEN, "%d", game->score.wave);
            break;
        case COINS:
            snprintf(itemValue, ITEM_VALUE_LEN, "%d", game->score.coins);
            break;
        case KILLS:
            snprintf(itemValue, ITEM_VALUE_LEN, "%d", game->score.kills);
            break;

        case NEW_GAME:
        case CUSTOM_GAME:
        case RESTORE_GAME:
        case OPTIONS:
        case EXIT:
        case START_GAME:
        case RESUME_GAME:
        case SAVE_QUIT:
        case QUIT_GAME:
        case STRING_LIST:
        case BACK:
            break;
    }
}

// MARK: - Update game data
void updateGameData(Game* game, MenuItem item, int incr) {
    switch(item) {
        // NOTE: Game settings
        case MAP_WIDTH:
            if (game->data.width < WIDTH_MAX && incr == 1) {
                game->data.width++;
            } else if (game->data.width > WIDTH_MIN && incr == -1) {
                game->data.width--;
            }
            break;
        case MAP_HEIGHT:
            if (game->data.height < HEIGHT_MAX && incr == 1) {
                game->data.height++;
            } else if (game->data.height > HEIGHT_MIN && incr == -1) {
                game->data.height--;
            }
            break;
        case SEED:
            if (game->data.seed < UINT_MAX && incr == 1) {
                game->data.seed++;
            } else if (game->data.seed > 0 && incr == -1) {
                game->data.seed--;
            }
            break;
        case SEASON:
            if (game->data.season < 3 && incr == 1) {
                game->data.season++;
            } else if (game->data.season > 0 && incr == -1) {
                game->data.season--;
            }
            break;
        case MIN_PATH_LENGTH:
            if (game->data.minPathLength < game->data.maxPathLength-1 && incr == 1) {
                game->data.minPathLength++;
            } else if (game->data.minPathLength > (game->data.height - LAND_WATER_RATIO) && incr == -1) {
                game->data.minPathLength--;
            }
            break;
        case MAX_PATH_LENGTH:
            if (game->data.maxPathLength < getMaxPathLength(game) && incr == 1) {
                game->data.maxPathLength++;
            } else if (game->data.maxPathLength > game->data.minPathLength+1 && incr == -1) {
                game->data.maxPathLength--;
            }
            break;
        case CROWN_HEALTH:
            if (game->data.crownHealth < UINT_MAX && incr == 1) {
                game->data.crownHealth++;
            } else if (game->data.crownHealth > 0 && incr == -1) {
                game->data.crownHealth--;
            }
            break;
        case FRAME_RATE:
            if (game->data.framerate < FRAMERATE_MAX && incr == 1) {
                game->data.framerate++;
            } else if (game->data.framerate > FRAMERATE_MIN && incr == -1) {
                game->data.framerate--;
            }
            game->data.refreshDelay = 1e6 / game->data.framerate;
            break;
        case SOUND:
            if (game->data.soundEnabled && incr == 1) {
                game->data.soundEnabled = 0;
            } else if (!game->data.soundEnabled && incr == -1) {
                game->data.soundEnabled = 1;
            }
            break;

        case WAVE:
        case COINS:
        case KILLS:
        case NEW_GAME:
        case CUSTOM_GAME:
        case RESTORE_GAME:
        case OPTIONS:
        case EXIT:
        case START_GAME:
        case RESUME_GAME:
        case SAVE_QUIT:
        case QUIT_GAME:
        case STRING_LIST:
        case BACK:
            break;
    }
}

// MARK: - Display horizontal menu
void displayHorizontalMenu(Game* game, int titleWidth, MenuItem* items, int numberOfItems, int itemsWidth, MenuItem* activeItem, int itemMarker, int onlyDisplay, char* pressedKey) {
    clearLine();

    // NOTE: List and shape menu items
    for (int i = 0; i < numberOfItems; i++) {
        for (int s = 0; s <= (titleWidth - (itemsWidth + 1))/(numberOfItems + 1); s++) {
            printf(" ");
        }

        // NOTE: Invert color for active item
        if (i == *activeItem) {
            invertColors();
            
            // NOTE: Display marker for active item
            if (itemMarker) {
                printf("* ");
            }
        }


        printf("%s", MENU_ITEMS[items[i]]);

        resetStyle();
    }
    
    if(!onlyDisplay) {
        // NOTE: Detect key press
        *pressedKey = getchar();
        if (*pressedKey == '\033') {
            getchar(); // NOTE: Skip [ character

            switch(getchar()) {
                case 'C': // NOTE: Arrow right
                    if (*activeItem < numberOfItems - 1) {
                        (*activeItem)++;
                    }
                    break;
                case 'D': // NOTE: Arrow left
                    if (*activeItem > 0) {
                        (*activeItem)--;
                    }
                    break;
            }

            displayHorizontalMenu(game, titleWidth, items, numberOfItems, itemsWidth, activeItem, itemMarker, onlyDisplay, pressedKey);

        // NOTE: Refresh if pressed key is not ENTER
        } else if (*pressedKey != '\r' && *pressedKey != '\n') {
            displayHorizontalMenu(game, titleWidth, items, numberOfItems, itemsWidth, activeItem, itemMarker, onlyDisplay, pressedKey);
        }
    } else { // NOTE: If only display, wait for ENTER key
        while(*pressedKey != '\r' && *pressedKey != '\n') {
            *pressedKey = getchar();
        }
    }
}

// MARK: - Display vertical menu
void displayVerticalMenu(Game* game, int titleWidth, MenuItem* items, char** stringItems, int numberOfItems, char* itemValue, MenuItem* activeItem, int itemMarker, int onlyDisplay, char* pressedKey) {
    // NOTE: List and shape items in vertical orientation
    for (int i = 0; i < numberOfItems; i++) {
        clearLine(); // NOTE: Clear line before displaying item

        // NOTE: Invert color for active item
        if (i == *activeItem) {
            invertColors();
            
            // NOTE: Display marker for active item
            if (itemMarker) {
                printf("* ");
            }
        }

        if (items == NULL) { // NOTE: If item is a string list, display the string
            if (i == numberOfItems - 1) { // NOTE: If item is BACK, display the string
                clearLine(); // NOTE: Remove marker
                printf("%s\n", MENU_ITEMS[BACK]);
            } else {
                printf("%d. %s", i + 1, stringItems[i]);
            }
        } else {
            printf("%s", MENU_ITEMS[items[i]]);

            if (items[i] <= KILLS) {
                setItemValue(game, items[i], itemValue); // NOTE: Set value format to diaply for item

                for (int s = 0; s <= titleWidth - strlen(MENU_ITEMS[items[i]]) - strlen(itemValue); s++) { // NOTE: Set number of space for "space between" format
                    printf(" ");
                }

                printf("%s\n", itemValue);
            } else {
                printf("\n");
            }
        }

        resetStyle();
    }
    moveCursorUp(numberOfItems); // NOTE: Move cursor to the first line of the menu

    if(!onlyDisplay) {
        // NOTE: Detect key press
        *pressedKey = getchar();
        if (*pressedKey == '\033') {
            getchar(); // NOTE: Skip [ character

            switch(getchar()) {
                case 'A': // NOTE: Arrow top
                    if (*activeItem > 0) {
                        (*activeItem)--;
                    }
                    break;
                case 'B': // NOTE: Arrow down
                    if (*activeItem < numberOfItems - 1) {
                        (*activeItem)++;
                    }
                    break;
                case 'C': // NOTE: Arrow right
                    updateGameData(game, items[*activeItem], 1);
                    break;
                case 'D': // NOTE: Arrow left
                    updateGameData(game, items[*activeItem], -1);
                    break;
            }

            displayVerticalMenu(game, titleWidth, items, stringItems, numberOfItems, itemValue, activeItem, itemMarker, onlyDisplay, pressedKey);
        } else if (items != NULL && items[*activeItem] <= SOUND && *pressedKey != '\r' && *pressedKey != '\n') { // NOTE: Refresh if item is not an action item or if pressed key is not ENTER
            displayVerticalMenu(game, titleWidth, items, stringItems, numberOfItems, itemValue, activeItem, itemMarker, onlyDisplay, pressedKey);
        }
    } else { // NOTE: If only display, wait for ENTER key
        while(*pressedKey != '\r' && *pressedKey != '\n') {
            *pressedKey = getchar();
        }
    }
}

int getItemsWidth(MenuItem* items, int numberOfItems) {
    int itemsWidth = 0;
    for (int i = 0; i < numberOfItems; i++) {
        itemsWidth += strlen(MENU_ITEMS[items[i]]);
    }

    return itemsWidth;
}

// MARK: - Menu
void menu(char* title, MenuDirection direction, Game* game, MenuItem* items, char** stringItems, int numberOfItems, MenuItem* selectedItem, int itemMarker, int onlyDisplay) {
    char pressedKey;
    char itemValue[ITEM_VALUE_LEN];
    int titleWidth;
    if(!onlyDisplay) *selectedItem = 0;

    clear();
    titleWidth = asciiArt(title);

    switch(direction) {
        case HORIZONTAL_MENU:
            displayHorizontalMenu(game, titleWidth, items, numberOfItems, getItemsWidth(items, numberOfItems), selectedItem, itemMarker, onlyDisplay, &pressedKey);
            break;

        case VERTICAL_MENU:
            displayVerticalMenu(game, titleWidth, items, stringItems, numberOfItems, itemValue, selectedItem, itemMarker, onlyDisplay, &pressedKey);
            break;
    }

    if (items != NULL) {
        *selectedItem = items[*selectedItem]; // NOTE: Set selectedItem to the real index of the selected item (according to MeniItems)
    } else if (stringItems != NULL && *selectedItem == numberOfItems - 1) { // Last item in string list is BACK
        *selectedItem = BACK; // NOTE: Set selectedItem to BACK index
    }
}

// MARK: - Main menu
void mainMenu(Game* game, MenuItem* selectedItem) {
    MenuItem items[] = {NEW_GAME, CUSTOM_GAME, RESTORE_GAME, OPTIONS, EXIT};

    menu("coconutParty", HORIZONTAL_MENU, game, items, NULL, MAIN_ITEMS, selectedItem, 1, 0);
}

// MARK: - Custom game menu
void customGameMenu(Game* game, MenuItem* selectedItem) {
    MenuItem items[] = {MAP_WIDTH, MAP_HEIGHT, SEED, SEASON, MIN_PATH_LENGTH, MAX_PATH_LENGTH, CROWN_HEALTH, START_GAME, BACK};

    menu("Custom", VERTICAL_MENU, game, items, NULL, CUSTOM_GAME_ITEMS, selectedItem, 0, 0);

    // If user select BACK item, reset game data to default values
    if (*selectedItem == BACK) {
        initGameData(game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGTH, DEFAULT_MAX_PATH_LENGTH, DEFAULT_MONKEY_AMOUNT, DEFAULT_CROWN_HEALTH, 1);
    }
}

// Options menu
void optionsMenu(Game* game, MenuItem* selectedItem) {
    MenuItem items[] = {FRAME_RATE, SOUND, BACK};
    
    menu("Options", VERTICAL_MENU, game, items, NULL, OPTIONS_ITEMS, selectedItem, 0, 0);
}

void restoreDisplay(Game* game) {
    clear();
    printTerrain(game);
    refreshScores(game);

    // NOTE: Restore background entities
    for (int i = 0; i < game->backgroundEntities.length; i++) {
        printBackgroundEntity(game, game->backgroundEntities.tab[i]);
    }

    // NOTE: Restore coins
    for (int i = 0; i < game->coins.length; i++) {
        Coin coin = game->coins.tab[i];
        if (coin.state == COIN_DISABLED) continue;
        printCoin(game, coin);
    }

    // NOTE: Restore crabs
    for (int i = 0; i < game->crabs.length; i++) {
        Crab crab = game->crabs.tab[i];
        if (crab.dead) continue;
        printCrab(game, crab);
    }

    // NOTE: Restore store
    printMonkeyShop(game);

    // NOTE: Restore monkeys
    for (int i = 0; i < game->monkeys.length; i++) {
        Monkey monkey = game->monkeys.tab[i];
        if (monkey.type == NOT_PLACED) continue;
        printMonkey(game, monkey);
    }
}

// MARK: - Pause menu
void pauseMenu(Game* game) {
    MenuItem items[] = {RESUME_GAME, OPTIONS, SAVE_QUIT, QUIT_GAME};
    MenuItem selectedItem;
    int resumeGame = 0;

    resetStyle();

    while (!resumeGame) {
        menu("Pause", VERTICAL_MENU, game, items, NULL, PAUSE_ITEMS, &selectedItem, 0, 0);

        switch(selectedItem) {
            case RESUME_GAME:
                restoreDisplay(game);
                resumeGame = 1;
                break;

            case OPTIONS:
                optionsMenu(game, &selectedItem);
                break;

            case SAVE_QUIT:
                break;

            case QUIT_GAME:
                game->end.poppedIndex = 0; // NOTE: Trigger end game condition
                resumeGame = 1;
                break;

            case MAP_WIDTH:
            case MAP_HEIGHT:
            case SEED:
            case SEASON:
            case MIN_PATH_LENGTH:
            case MAX_PATH_LENGTH:
            case CROWN_HEALTH:
            case FRAME_RATE:
            case SOUND:
            case NEW_GAME:
            case CUSTOM_GAME:
            case RESTORE_GAME:
            case EXIT:
            case START_GAME:
            case STRING_LIST:
            case BACK:
                break;
        }
    }
}

void nextShopMenu(Game* game, int direction) {
    MonkeyShopMenu* selected = &game->monkeys.shop.focusedMenu;
    (*selected) += direction;
    if (*selected > SHOP_BUY) {
        *selected = SHOP_TYPE;
    } else if (*selected < SHOP_TYPE) {
        *selected = SHOP_BUY;
    }
}

void nextMonkeyType(Game* game, int direction) {
    MonkeyType* selected = &game->monkeys.shop.selectedType;
    (*selected) += direction;
    if (*selected > STUNNER) {
        *selected = ALPHA;
    } else if (*selected < ALPHA) {
        *selected = STUNNER;
    }
}

void nextMonkey(Game* game, int direction) {
    int* selected = &game->monkeys.shop.selectedMonkey;
    (*selected) += direction;
    if (*selected > game->monkeys.length - 1) {
        *selected = 0;
    } else if (*selected < 0) {
        *selected = game->monkeys.length - 1;
    }
}

void listenToKeyboard(Game* game) {
    fd_set readfds; // NOTE: File descriptor set (File descriptor = int value used to identify a file, socket or device)
    struct timeval timeout;

    FD_ZERO(&readfds); // NOTE: Init the file descriptor set to zero
    FD_SET(STDIN_FILENO, &readfds); // NOTE: Add keyboard input to the file descriptor set

    // NOTE: Wait input for 1us
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
    int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    // NOTE: Check if there is input on keyboard
    if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
        char c = getchar();
        if (c == '\033') {
            c = getchar(); // NOTE: Skip [ character
            switch ((c = getchar())) {
                case 'A': // NOTE: Up arrow
                    nextShopMenu(game, -1);
                    printMonkeyShop(game);
                    break;

                case 'B': // NOTE: Down arrow
                    nextShopMenu(game, 1);
                    printMonkeyShop(game);
                    break;

                case 'D': // NOTE: Left arrow
                    if (game->monkeys.shop.focusedMenu == SHOP_TYPE) {
                        nextMonkeyType(game, -1);
                    } else if (game->monkeys.shop.focusedMenu == SHOP_MONKEY) {
                        nextMonkey(game, -1);
                    }
                    printMonkeyShop(game);
                    break;

                case 'C': // NOTE: Right arrow
                    if (game->monkeys.shop.focusedMenu == SHOP_TYPE) {
                        nextMonkeyType(game, 1);
                    } else if (game->monkeys.shop.focusedMenu == SHOP_MONKEY) {
                        nextMonkey(game, 1);
                    }
                    printMonkeyShop(game);
                    break;
            }
        } else if (c == ' ') {
            pauseMenu(game);
        } else if ((c == '\r' || c == '\n') && game->monkeys.tab[game->monkeys.shop.selectedMonkey].type == NOT_PLACED) {
            buyMonkey(game);
            printMonkeyShop(game);
        }
    }
}

// MARK: - End game menu
void endGameMenu(Game* game) {
    MenuItem selectedItem = END_GAME_ITEMS-1;
    MenuItem items[] = {WAVE, COINS, KILLS, BACK};

    menu("GameOver", VERTICAL_MENU, game, items, NULL, END_GAME_ITEMS, &selectedItem, 0, 1);
}