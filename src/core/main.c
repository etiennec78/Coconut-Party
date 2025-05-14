#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "core/common.h"
#include "entities/backgroundEntities.h"
#include "entities/coins.h"
#include "entities/crabs.h"
#include "entities/monkeys.h"
#include "graphics/asciiArt.h"
#include "graphics/display.h"
#include "modules/menus.h"
#include "modules/storage.h"
#include "modules/terrain.h"

void createGame(Game* game) {
    clear();
    asciiArt("Loading");

    createBackgroundEntities(game);
    createTerrain(game);
    createCrabs(game);
    createCoins(game);
}

void waitFrame(Game* game, struct timeval startTime) {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    double elapsed = (currentTime.tv_sec - startTime.tv_sec) * 1e6 +
                     (currentTime.tv_usec - startTime.tv_usec);

    double sleepTime = game->data.refreshDelay - elapsed;

    if (sleepTime > 0) {
        usleep(sleepTime);
    }
}

void refreshGame(Game* game) {
    struct timeval startTime;
    gettimeofday(&startTime, NULL);

    updateCrabs(game);
    updateCoins(game);
    updateCrown(game);
    updateMonkeys(game);
    updateBackgroundEntities(game);

    fflush(stdout); // Flush buffer to print without delay

    waitFrame(game, startTime);
}

void freeGame(Game* game) {
    freeTerrain(game->terrain);
    free(game->path.tab);
    free(game->crabs.tab);
    free(game->monkeys.tab);
}

void exitGame() {
    clear();
    resetStyle();
    showCursor();
    setRawMode(0);
}

void runGame(Game *game, int fromMenu) {
    clear();

    printTerrain(game);
    refreshScores(game);
    if (!fromMenu) {
        startWave(game);
    } else {
        restoreDisplay(game);
    }

    while (game->end.poppedIndex > 0) {
        refreshGame(game);
        listenToKeyboard(game);
    }

    if(!game->header.isAlreadySaved) endGameMenu(game);
}

int main() {
    Game game;
    MenuItem selectedItem, quit = 0;

    srand(time(NULL));
    hideCursor();
    setRawMode(1); // NOTE: Enable row mode
    resetStyle();

    initGameData(&game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGTH, DEFAULT_MAX_PATH_LENGTH, DEFAULT_MONKEY_AMOUNT, DEFAULT_CROWN_HEALTH, 0);

    while (!quit) {
        mainMenu(&game, &selectedItem);

        switch(selectedItem) {
            case NEW_GAME:
                initGameData(&game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGTH, DEFAULT_MAX_PATH_LENGTH, DEFAULT_MONKEY_AMOUNT, DEFAULT_CROWN_HEALTH, 0);
                createGame(&game);
                runGame(&game, 0);
                freeGame(&game);
                break;

            case CUSTOM_GAME:
                initGameData(&game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGTH, DEFAULT_MAX_PATH_LENGTH, DEFAULT_MONKEY_AMOUNT, DEFAULT_CROWN_HEALTH, 0);
                customGameMenu(&game, &selectedItem);
                
                if (selectedItem == START_GAME) {
                    createGame(&game);
                    runGame(&game, 0);
                    freeGame(&game);
                }
                break;

            case RESTORE_GAME:
                if(restoreGameMenu(&game, &selectedItem) && selectedItem != BACK) {
                    createGame(&game);
                    restoreGame(&game, selectedItem);
                    runGame(&game, 1);
                    freeGame(&game);
                }
                break;

            case OPTIONS:
                optionsMenu(&game, &selectedItem);
                break;

            case EXIT:
                exitGame();
                asciiArt("CocoBye");
                quit = 1;
                break;

            default:
                printf("🚨 Your selection has created an error (main menu) !\n");
                exit(3);
                break;
        }
    }
    
    return 0;
}

