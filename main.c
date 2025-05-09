#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "crabs.h"
#include "display.h"
#include "terrain.h"
#include "monkeys.h"
#include "backgroundEntities.h"
#include "coins.h"

#define WIDTH 40
#define HEIGHT 30

void createGame(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength) {
    game->data.width = width;
    game->data.height = height;
    game->data.endHeight = (1 - LAND_WATER_RATIO) * height + WATER_MAX_RANDOMNESS + height * FINISH_LINE_RATIO;
    game->data.seed = seed;
    game->data.season = AUTUMN;
    game->data.minPathLength = minPathLength;
    game->data.maxPathLength = maxPathLength;
    game->data.monkeyAmount = 15;
    game->data.crownHealth = 100;
    game->data.backoff.maxTime = 3;
    game->data.backoff.maxTries = 4;
    game->data.backoff.multiplier = 5;
    game->data.framerate = 30;
    game->data.refreshDelay = 1e6 / game->data.framerate;
    game->data.soundEnabled = 1;

    game->score.wave = 0;
    game->score.coins = 0;
    game->score.kills = 0;
    game->score.remainingCrabs = 0;

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
    free(game->monkeys.tab);
}

int main() {
    Game game;
    unsigned int seed = time(NULL);
    int minPathLength = 30;
    int maxPathLength = 200;

    createGame(&game, WIDTH, HEIGHT, seed, minPathLength, maxPathLength);
    resetColorBackground();
    hideCursor();
    printTerrain(&game);
    refreshScores(&game);
    startWave(&game, 5);
    refreshScores(&game);

    while (game.crown.health > 0) {
        refreshGame(&game);
    }

    exitGame(&game);
    return 0;
}
