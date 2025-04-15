#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"
#include "terrain.h"

#define WIDTH 40
#define HEIGHT 30

void createGame(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength) {
    game->data.width = width;
    game->data.height = height;
    game->data.seed = seed;
    game->data.minPathLength = minPathLength;
    game->data.maxPathLength = maxPathLength;

    createTerrain(game);
}

int main() {
    Game game;
    // unsigned int seed = 10; // time(NULL);
    int minPathLength = 30;
    int maxPathLength = 200;


    for (int i=0; i<10; i++) {
        unsigned int seed = time(NULL);
        printf("seed %d\n", seed);
        createGame(&game, WIDTH, HEIGHT, seed, minPathLength, maxPathLength);
        printGame(&game);
    }

    // createGame(&game, WIDTH, HEIGHT, seed, minPathLength, maxPathLength);
    //
    // printGame(&game);

    freeTerrain(game.terrain);
    free(game.path.tab);
    return 0;
}
