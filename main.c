#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    game->data.minPathLength = minPathLength;
    game->data.maxPathLength = maxPathLength;
    game->data.backoff.maxTime = 3;
    game->data.backoff.maxTries = 4;
    game->data.backoff.multiplier = 5;

    createTerrain(game);
    createCrabs(game, 1);
}

int main() {
    Game game;
    unsigned int seed = time(NULL);
    int minPathLength = 30;
    int maxPathLength = 200;

    createGame(&game, WIDTH, HEIGHT, seed, minPathLength, maxPathLength);

    printGame(&game);

    freeTerrain(game.terrain);
    free(game.path.tab);
    free(game.crabs.tab);
    return 0;
}
