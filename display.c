#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void printTerrain(char** terrain, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%s", SEASONS[season][terrain[y][x]]);
        }
        printf("\n");
    }
}

void printGame(Game* game) {
    printTerrain(game->terrain, game->data.width, game->data.height);
}
