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
#include "storage.h"
#include "terrain.h"

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
            snprintf(itemValue, ITEM_VALUE_LEN, "< %s >", seasonItems[game->data.season]);
            break;
        case MIN_PATH_LENGHT:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.minPathLength);
            break;
        case MAX_PATH_LENGHT:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.maxPathLength);
            break;
        case CROWN_HEALTH:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.crownHealth);
            break;
        case FRAME_RATE:
            snprintf(itemValue, ITEM_VALUE_LEN, "< %d >", game->data.framerate);
            break;
        case SOUND:
            if(game->data.soundEnabled) {
                snprintf(itemValue, ITEM_VALUE_LEN, "< On >");
            } else {
                snprintf(itemValue, ITEM_VALUE_LEN, "< Off >");
            }
            break;

        // NOTE: Main menus
        case NEW_GAME:
        case CUSTOM_GAME:
        case RESTORE_GAME:
        case OPTIONS:
        case EXIT:
        
        // NOTE: Game actions
        case START_GAME:
        case RESUME_GAME:
        case SAVE_QUIT:
        case QUIT_GAME:
        
        // NOTE: Common items
        case BACK:
            break;

        default:
            printf("🚨 An error occurred while displaying option values ! (#%d)\n", item);
            exit(1);
    }
}
void updateGameDatas(Game* game, MenuItem item, int incr) { // NOTE: Update game datas
    switch(item) {
        // NOTE: Game settings
        case MAP_WIDTH:
            if(game->data.width < WIDTH_MAX && incr == 1) {
                game->data.width++;
            } else if(game->data.width > WIDTH_MIN && incr == -1) {
                game->data.width--;
            }
            break;
        case MAP_HEIGHT:
            if(game->data.height < HEIGHT_MAX && incr == 1) {
                game->data.height++;
            } else if(game->data.height > HEIGHT_MIN && incr == -1) {
                game->data.height--;
            }
            break;
        case SEED:
            if(game->data.seed < UINT_MAX && incr == 1) { // NOTE: Valeur max de unsigned int
                game->data.seed++;
            } else if(game->data.seed > 0 && incr == -1) {
                game->data.seed--;
            }
            break;
        case SEASON:
            if(game->data.season < 3 && incr == 1) {
                game->data.season++;
            } else if(game->data.season > 0 && incr == -1) {
                game->data.season--;
            }
            break;
        case MIN_PATH_LENGHT:
            if(game->data.minPathLength < game->data.maxPathLength-1 && incr == 1) {
                game->data.minPathLength++;
            } else if(game->data.minPathLength > (game->data.height - LAND_WATER_RATIO) && incr == -1) {
                game->data.minPathLength--;
            }
            break;
        case MAX_PATH_LENGHT:
            if(game->data.maxPathLength < getMaxPathLength(game) && incr == 1) {
                game->data.maxPathLength++;
            } else if(game->data.maxPathLength > game->data.minPathLength+1 && incr == -1) {
                game->data.maxPathLength--;
            }
            break;
        case CROWN_HEALTH:
            if(game->data.crownHealth < UINT_MAX && incr == 1) {
                game->data.crownHealth++;
            } else if(game->data.crownHealth > 0 && incr == -1) {
                game->data.crownHealth--;
            }
            break;
        case FRAME_RATE:
            if(game->data.framerate < FRAMERATE_MAX && incr == 1) {
                game->data.framerate++;
            } else if(game->data.framerate > FRAMERATE_MIN && incr == -1) {
                game->data.framerate--;
            }
            game->data.refreshDelay = 1e6 / game->data.framerate;
            break;
        case SOUND:
            if(game->data.soundEnabled && incr == 1) {
                game->data.soundEnabled = 0;
            } else if(!game->data.soundEnabled && incr == -1) {
                game->data.soundEnabled = 1;
            }
            break;

        // NOTE: Main menus
        case NEW_GAME:
        case CUSTOM_GAME:
        case RESTORE_GAME:
        case OPTIONS:
        case EXIT:
        
        // NOTE: Game actions
        case START_GAME:
        case RESUME_GAME:
        case SAVE_QUIT:
        case QUIT_GAME:
        
        // NOTE: Common items
        case BACK:
            break;

        default:
            printf("🚨 An error occurred during the update of game datas ! (#%d)\n", item);
            exit(1);
    }
}







// MARK: - Display horizontal menu
void displayHorizontalMenu(Game* game, MenuItem* items, int numberOfItems, int* itemsWidth, int* activeItem, int itemMarker, char* pressedKey) {
    clearLine(); // NOTE: Clear line before display item

    // NOTE: List and shape menu items
    for(int i=0; i<numberOfItems; i++) { // TODO: 144
        for(int s=0; s<=(114-(*itemsWidth))/(MAIN_ITEMS+1); s++) {
            printf(" ");
        }

        // NOTE: Invert color for active item
        if(i == *activeItem) {
            color("7");
            
            // NOTE: Display marker for active item
            if(itemMarker) {
                printf("* ");
            }
        }


        printf("%s", menuItems[items[i]]);

        color("0");
    }
    
    // NOTE: Detect keyboard key
    *pressedKey = getchar();
    if(*pressedKey == '\033') {
        getchar(); // NOTE: Skip [ char

        switch(getchar()) {
            case 'C': // NOTE: Arrow right
                if(*activeItem < numberOfItems-1) {
                    (*activeItem)++;
                }
                break;
            case 'D': // NOTE: Arrow left
                if(*activeItem > 0) {
                    (*activeItem)--;
                }
                break;
        }

        displayHorizontalMenu(game, items, numberOfItems, itemsWidth, activeItem, itemMarker, pressedKey);
    } else if(*pressedKey != '\r' && *pressedKey != '\n') { // NOTE: Refresh if pressed key is not ENTER 
        displayHorizontalMenu(game, items, numberOfItems, itemsWidth, activeItem, itemMarker, pressedKey);
    }
}

// MARK: - display vertical menu
void displayVerticalMenu(Game* game, MenuItem* items, char** stringItems, int numberOfItems, char* itemValue, int* activeItem, int itemMarker, char* pressedKey) {
    // NOTE: List and shape items in vertical orientation
    for(int i=0; i<numberOfItems; i++) {
        clearLine(); // NOTE: Clear line before display item

        // NOTE: Invert color for active item
        if(i == *activeItem) {
            color("7");
            
            // NOTE: Display marker for active item
            if(itemMarker) {
                printf("* ");
            }
        }

        if(items == NULL) { // NOTE: If item is a string list, display the string
            if(i == numberOfItems-1) { // NOTE: If item is BACK, display the string
                clearLine(); // NOTE: Remove marker
                printf("%s\n", menuItems[BACK]);
            } else {
                printf("%d. %s\n", i+1, stringItems[i]);
            }
        } else {
            printf("%s", menuItems[items[i]]);

            if(items[i] <= SOUND) { // TOOD: 68
                setItemValue(game, items[i], itemValue); // NOTE: Set value format to diaply for item

                for(int s=0; s<=68-strlen(menuItems[items[i]])-strlen(itemValue); s++) { // NOTE: Set number of space for "space between" format
                    printf(" ");
                }

                printf("%s\n", itemValue);
            } else {
                printf("\n");
            }
        }

        color("0");
    }
    prevLine(numberOfItems); // NOTE: Move cursor to the first line of the menu

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
                updateGameDatas(game, items[*activeItem], 1);
                break;
            case 'D': // NOTE: Arrow left
                updateGameDatas(game, items[*activeItem], -1);
                break;
        }

        displayVerticalMenu(game, items, stringItems, numberOfItems, itemValue, activeItem, itemMarker, pressedKey);
    } else if(items != NULL && items[*activeItem] <= SOUND && (*pressedKey != '\r' || *pressedKey != '\n')) { // NOTE: Refresh if item is not an action item or if pressed key is not ENTER
        displayVerticalMenu(game, items, stringItems, numberOfItems, itemValue, activeItem, itemMarker, pressedKey);
    }
}
// MARK: - Menu
void menu(char* title, MenuDirection direction, Game* game, MenuItem* items, char** stringItems, int numberOfItems, int* selectedItem, int itemMarker) {
    char* pressedKey = malloc(sizeof(char));
    if(pressedKey == NULL) {
        printf("🚨 Memory allocation failed for pressedKey (vertical menu) !\n");
        exit(1);
    }

    *selectedItem = 0;

    clear();
    asciiArt(title);

    switch(direction) {
        case HORIZONTAL_MENU:
            int *itemsWidth = malloc(sizeof(int));
            if(itemsWidth == NULL) {
                printf("🚨 Memory allocation failed for itemsWidth !\n");
                exit(1);
            }

            // NOTE: Calcul total characters width of items
            *itemsWidth = 0;
            for(int iw=0; iw<numberOfItems; iw++) {
                *itemsWidth += strlen(menuItems[items[iw]]);
            }

            displayHorizontalMenu(game, items, numberOfItems, itemsWidth, selectedItem, itemMarker, pressedKey);
            break;
        case VERTICAL_MENU:
            char* itemValue = malloc(ITEM_VALUE_LEN * sizeof(char));
            if(itemValue == NULL) {
                printf("🚨 Memory allocation failed for itemValue !\n");
                exit(1);
            }

            displayVerticalMenu(game, items, stringItems, numberOfItems, itemValue, selectedItem, itemMarker, pressedKey);
            
            free(itemValue);
            break;
    }

    if(items != NULL) {
        *selectedItem = items[*selectedItem]; // NOTE: Set selectedItem to the real index of the selected item (according to MeniItems)
    }

    free(pressedKey);
}







// MARK: - Main menu
void mainMenu(Game* game, int* selectedItem) {
    MenuItem* items = malloc(MAIN_ITEMS * sizeof(MenuItem));
    if(items == NULL) {
        printf("🚨 Memory allocation failed for items (options menu) !\n");
        exit(1);
    }
    
    // NOTE: Create items array
    items[0] = NEW_GAME;
    items[1] = CUSTOM_GAME;
    items[2] = RESTORE_GAME;
    items[3] = OPTIONS;
    items[4] = EXIT;

    menu("coconutParty", HORIZONTAL_MENU, game, items, NULL, MAIN_ITEMS, selectedItem, 1);
}

// MARK: - Custom game menu
void customGameMenu(Game* game, int* selectedItem) {
    MenuItem* items = malloc(CUSTOM_GAME_ITEMS * sizeof(MenuItem));
    if(items == NULL) {
        printf("🚨 Memory allocation failed for items (custom menu) !\n");
        exit(1);
    }

    // NOTE: Create items array
    items[0] = MAP_WIDTH;
    items[1] = MAP_HEIGHT;
    items[2] = SEED;
    items[3] = SEASON;
    items[4] = MIN_PATH_LENGHT;
    items[5] = MAX_PATH_LENGHT;
    items[6] = CROWN_HEALTH;
    items[7] = START_GAME;
    items[8] = BACK;

    menu("Custom", VERTICAL_MENU, game, items, NULL, CUSTOM_GAME_ITEMS, selectedItem, 0);

    if(*selectedItem == BACK) { // NOTE: If user select BACK item, reset game data to default values
        initGameDatas(game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGHT, DEFAULT_MAX_PATH_LENGHT, DEFAULT_CROWN_HEALTH, 1);
    }
    
    free(items);
}

// MARK: - Restore game menu
int restoreGameMenu(Game* game, int* selectedItem) {
    int countGameSaved = countSavedGames(); // NOTE: Count the number of line in the file (1 line = 1 game name)
    
    if(countGameSaved != -1) { // NOTE: If there is at least one game saved
        Header* gamesHeaderList = malloc(countGameSaved * sizeof(Header)); // NOTE: Allocate memory for stock games name
        listGame(gamesHeaderList); // NOTE: Set save game list

        char** gamesNameList = malloc(countGameSaved * sizeof(char*)); // NOTE: Allocate memory for stock games name
        if(gamesNameList == NULL) {
            printf("🚨 Memory allocation failed for gamesNameList (restore game menu) !\n");
            exit(1);
        }

        // Allocate memory for each game name (gn) in the list
        for(int gn=0; gn<countGameSaved; gn++) {
            gamesNameList[gn] = malloc(GAME_NAME_LEN_MAX * sizeof(char)); // NOTE: Allocate memory for stock one game name
            if (gamesNameList[gn] == NULL) {
                printf("🚨 Memory allocation failed for gamesNameList[%d] !\n", gn);
                exit(1);
            }

            strncpy(gamesNameList[gn], gamesHeaderList[gn].name, GAME_NAME_LEN_MAX); // NOTE: Copy games name to gameNamesList
        }

        menu("RestoreGame", VERTICAL_MENU, game, NULL, gamesNameList, countGameSaved+1, selectedItem, 0);

        // NOTE: Free game names list
        for (int gn=0; gn<countGameSaved; gn++) {
            free(gamesNameList[gn]);
        }
        free(gamesNameList);

        // NOTE: Set selectedItem to the ID of the selected game if it is not BACK
        if(*selectedItem != countGameSaved) {
            *selectedItem = gamesHeaderList[*selectedItem].id;
            
            free(gamesHeaderList);
            return 1; // NOTE: Return 1 (True) for restored/restart selected game
        }
        
        free(gamesHeaderList);
        return 0;

    } else {
        clear();
        asciiArt("RestoreGame");

        char* pressedKey = malloc(sizeof(char));
        if(pressedKey == NULL) {
            printf("🚨 Memory allocation failed for pressedKey (vertical menu) !\n");
            exit(1);
        }

        // NOTE: Center noGameSavedMessage
        for(int space=0; space<(112-strlen(noGameSavedMessage))/2; space++) {
            printf(" ");
        }

        printf("%s", noGameSavedMessage);
        
        *pressedKey = getchar();
        if(*pressedKey == 8 || *pressedKey == 127) { // NOTE: Backspace key (Window = 8, Linux = 127)
            free(pressedKey);
            return 0; // NOTE: Return 0 (False) if game name list is empty
        } else {
            free(pressedKey);

            clearLine();
            restoreGameMenu(game, selectedItem);
        }
    }
}

// MARK: - Options menu
void optionsMenu(Game* game, int* selectedItem) {
    MenuItem* items = malloc(OPTIONS_ITEMS * sizeof(MenuItem));
    if(items == NULL) {
        printf("🚨 Memory allocation failed for items (options menu) !\n");
        exit(1);
    }
    
    // NOTE: Create items array
    items[0] = FRAME_RATE;
    items[1] = SOUND;
    items[2] = BACK;
    
    menu("Options", VERTICAL_MENU, game, items, NULL, OPTIONS_ITEMS, selectedItem, 0);
    
    free(items);
}

// MARK: - Pause menu
void pauseMenu(Game* game) {
    fd_set readfds; // NOTE: File descriptor set (File descriptor = int value used to identify a file, socket or device)
    struct timeval timeout;
    int ret, selectedItem, resumeGame = 0;
    
    MenuItem* items = malloc(PAUSE_ITEMS * sizeof(MenuItem));
    if(items == NULL) {
        printf("🚨 Memory allocation failed for items (pause menu) !\n");
        exit(1);
    }  
    
    // NOTE: Create items array
    items[0] = RESUME_GAME;
    items[1] = OPTIONS;
    items[2] = SAVE_QUIT;
    items[3] = QUIT_GAME;

    FD_ZERO(&readfds); // NOTE: Init the file descriptor set to zero
    FD_SET(STDIN_FILENO, &readfds); // NOTE: Add keyboard input to the file descriptor set
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000; // 1ms timeout
    ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout); // NOTE: Wait input during 1ms

    if(ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) { // NOTE: Check if there is input on keyboard
        if(getchar() == ' ') { // NOTE: Space key
            resetColorBackground();

            while(!resumeGame) {
                menu("Pause", VERTICAL_MENU, game, items, NULL, PAUSE_ITEMS, &selectedItem, 0);

                switch(selectedItem) {
                    case RESUME_GAME:
                        clear();
                        printTerrain(game);
                        resumeGame = 1;
                        break;
                    case OPTIONS:
                        optionsMenu(game, &selectedItem);
                        break;
                    case SAVE_QUIT:
                        savedGame(game); // NOTE: Save game
                    case QUIT_GAME:
                        clear();
                        game->crown.health = 0; // NOTE: Set crown health to 0 to end game
                        resumeGame = 1;
                        break;
                }
            }
        }
    }

    free(items);
}




















































