#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "crabs.h"
#include "display.h"
#include "terrain.h"

#define WIDTH 90
#define HEIGHT 40

void createGame(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength) {
    game->data.width = width;
    game->data.height = height;
    game->data.endHeight = (1 - LAND_WATER_RATIO) * height + WATER_MAX_RANDOMNESS + height * FINISH_LINE_RATIO;
    game->data.seed = seed;
    game->data.season = AUTUMN;
    game->data.minPathLength = minPathLength;
    game->data.maxPathLength = maxPathLength;
    game->data.crownHealth = 100;
    game->data.backoff.maxTime = 3;
    game->data.backoff.maxTries = 4;
    game->data.backoff.multiplier = 5;
    game->data.framerate = 30;
    game->data.refreshDelay = 1e6 / game->data.framerate;
    game->data.soundEnabled = 1;

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

void exitGame(Game* game) {
    Coordinates screenBottom;
    screenBottom.x = 0;
    screenBottom.y = game->data.height;

    resetColorBackground();
    moveEmojiCursor(screenBottom);
    showCursor();

    freeTerrain(game->terrain);
    free(game->path.tab);
    free(game->crabs.tab);
}

int main() {
    Game game;
    unsigned int seed = time(NULL);
    int minPathLength = 30;
    int maxPathLength = 200;

    createGame(&game, WIDTH, HEIGHT, seed, minPathLength, maxPathLength);
    hideCursor();
    printTerrain(&game);
    startWave(&game, 5);

    while (game.crown.health > 0) {
        refreshGame(&game);
    }

    exitGame(&game);
    return 0;
}
