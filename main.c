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
#include "asciiArt.h"

void createGame(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength) {
    initGameDatas(game, width, height, seed, minPathLength, maxPathLength);
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
    }

    exitGame(game);
}

int main() {
    Game game;
    Options* items = NULL;
    int selectedMenu = 0, selectedOption = 0, out = 0;

    hideCursor();
    createGame(&game, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SEED, DEFAULT_MIN_PATH_LENGHT, DEFAULT_MAX_PATH_LENGHT);
    resetColorBackground();

    while(!out) {
        mainMenu(&selectedMenu);

        switch(selectedMenu) {
            case 0: // NOTE: New game
                runGame(&game);
                out = 1;
                break;
            case 1: // NOTE: Restore game
                selectedOption = 0;
                items = malloc((OPTIONS_ITEMS-2) * sizeof(Options));
                if(items == NULL) {
                    printf("🚨 An error occurred during the memory allocation for the options menu !\n");
                    exit(1);
                }
                items[0] = MAP_WIDTH;
                items[1] = MAP_HEIGHT;
                items[2] = SEED;
                items[3] = SEASON;
                items[4] = MIN_PATH_LENGHT;
                items[5] = MAX_PATH_LENGHT;
                items[6] = CROWN_HEALTH;
                items[7] = START_CUSTOM_GAME;
                items[8] = BACK;

                optionsMenu("Custom", &game, items, OPTIONS_ITEMS-2, &selectedOption);
                
                if(selectedOption == START_CUSTOM_GAME) {
                    runGame(&game);
                    out = 1;
                }
                
                free(items);
                items = NULL;
                break;
            case 2: // NOTE: Restore game
                clear();
                printf("Restore game");
                break;
            case 3: // NOTE: Options menu
                selectedOption = 0;
                items = malloc(3 * sizeof(Options));
                if(items == NULL) {
                    printf("🚨 An error occurred during the memory allocation for the options menu !\n");
                    exit(1);
                }
                items[0] = FRAME_RATE;
                items[1] = SOUND;
                items[2] = BACK;

                optionsMenu("Options", &game, items, 3, &selectedOption);

                free(items);
                items = NULL;
                break;
            case 4: // NOTE: Exit
                clear();
                asciiArt("CocoBye");
                out = 1;
                break;
            default:
                printf("🚨 Your selection create an error !\n");
                out = 1;
                break;
        }
    }
    
    freeGame(&game);
    return 0;
}

