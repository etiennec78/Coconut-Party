#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"
#include "terrain.h"

#define WIDTH 40
#define HEIGHT 30

void createGame(Game *game, int width, int height, unsigned int seed) {
    game->data.width = width;
    game->data.height = height;
    game->data.seed = seed;
    game->terrain = createTerrain(game);
}

int main() {
    Game game;
    unsigned int seed = time(NULL);

    createGame(&game, WIDTH, HEIGHT, seed);

    printGame(&game);

    freeTerrain(game.terrain);
    return 0;
}
