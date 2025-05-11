#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "common.h"
#include "asciiArt.h"
#include "crabs.h"
#include "display.h"
#include "menus.h"
#include "terrain.h"
#include "monkeys.h"
#include "backgroundEntities.h"
#include "coins.h"

void createGame(Game* game) {
    clear();
    asciiArt("Loading");

    createBackgroundEntities(game);
    createTerrain(game);
    createCrabs(game);
    createCoins(game);
}

void startWave(Game* game, int amount) {
    game->crabs.awaitingSpawn = amount;
    game->score.remainingCrabs = amount;
    game->score.wave++;

    printScore(UI_WAVE, game->score.wave);
    printScore(UI_ALIVE, game->score.remainingCrabs);
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

void runGame(Game *game) {
    clear();

    printTerrain(game);
    refreshScores(game);
    startWave(game, 5);

    while (game->crown.health > 0) {
        refreshGame(game);
        pauseMenu(game);
    }
}

int main() {
    Game game;
    MenuItem selectedItem, quit = 0;

    hideCursor();
    setRawMode(1); // NOTE: Enable row mode
    resetStyle();

    initGameData(&game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGTH, DEFAULT_MAX_PATH_LENGTH, DEFAULT_MONKEY_AMOUNT, DEFAULT_CROWN_HEALTH, 0);

    while (!quit) {
        mainMenu(&game, &selectedItem);

        switch(selectedItem) {
            case NEW_GAME:
                createGame(&game);
                runGame(&game);
                freeGame(&game);

                game.data.crownHealth = DEFAULT_CROWN_HEALTH;
                break;

            case CUSTOM_GAME:
                customGameMenu(&game, &selectedItem);
                
                if (selectedItem == START_GAME) {
                    createGame(&game);
                    runGame(&game);
                    freeGame(&game);

                    game.data.crownHealth = DEFAULT_CROWN_HEALTH;
                }
                break;

            case RESTORE_GAME:
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
                exit(1);
                break;
        }
    }

    return 0;
}

