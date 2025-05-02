#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void printTerrain(char** terrain, int width, int height) {
    int season = 2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%s", SEASONS[season][terrain[x][y]]);
        }
        printf("\n");
    }
}

void printGame(Game* game) {
    printTerrain(game->terrain, game->data.width, game->data.height);
}
