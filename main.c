#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "terrain.h"

#define WIDTH 40
#define HEIGHT 30

void createGame(Game *game, int width, int height) {
    game->data.width = width;
    game->data.height = height;
    game->terrain = createTerrain(game);
}

int main() {
    Game game;
    createGame(&game, WIDTH, HEIGHT);

    printGame(&game);

    free(game.terrain);
    return 0;
}
