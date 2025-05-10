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

void createGame(Game* game) {
    asciiArt("Loading");
    
    game->id = rand();
    createTerrain(game);
    createCrabs(game, 1);
}

void startWave(Game* game, int amount) {
    game->crabs.awaitingSpawn = amount;
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
    updateCrown(game);

    fflush(stdout); // Flush buffer to print without delay

    waitFrame(game, startTime);
}

void freeGame(Game* game) {
    freeTerrain(game->terrain);
    free(game->path.tab);
    free(game->crabs.tab);
}

void exitGame(Game* game) {
    Coordinates screenBottom;
    screenBottom.x = 0;
    screenBottom.y = game->data.height;

    resetColorBackground();
    moveEmojiCursor(screenBottom);
    showCursor();
}

void runGame(Game *game) {
    clear();

    printTerrain(game);
    startWave(game, 5);

    while (game->crown.health > 0) {
        refreshGame(game);
        pauseMenu(game);
    }

    exitGame(game);
}

int main() {
    Game game;
    int selectedItem, quit = 0;

    hideCursor();
    setRawMode(1); // NOTE: Enable row mode
    resetColorBackground();

    initGameDatas(&game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGHT, DEFAULT_MAX_PATH_LENGHT, DEFAULT_CROWN_HEALTH, 0);

    while(!quit) {
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
                
                if(selectedItem == START_GAME) {
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
                clear();
                asciiArt("CocoBye");
                quit = 1;
                break;
            default:
                printf("\n🚨 Your selection create an error (main menu) !\n");
                exit(1);
                break;
        }
    }

    return 0;
}

