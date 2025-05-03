#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
    game->data.crownHealth = 100;
    game->data.backoff.maxTime = 3;
    game->data.backoff.maxTries = 4;
    game->data.backoff.multiplier = 5;

    createTerrain(game);
    createCrabs(game, 1);
}

void startWave(Game* game, int amount) {
    game->crabs.awaitingSpawn = amount;
}

void refreshGame(Game* game) {
    // Erase old crabs
    for (int i = 0; i < game->crabs.length; i++) {
        moveEmojiCursor(game->crabs.tab[i].coord);
        printTerrainTile(game, game->crabs.tab[i].coord);
    }

    moveCrabs(game);

    printCrabs(game);
    fflush(stdout); // Flush buffer to print without delay

    usleep(500000);
}

void exitGame(Game* game) {
    Coordinates screenBottom;
    screenBottom.x = 0;
    screenBottom.y = game->data.height;

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
    printTerrain(game.terrain, game.data.width, game.data.height);

    startWave(&game, 5);

    while (game.crown.health > 0) {
        refreshGame(&game);
    }

    exitGame(&game);
    return 0;
}
