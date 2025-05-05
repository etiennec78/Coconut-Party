#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void printTerrain(char** terrain, int width, int height) {
    int season = 3;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            printf("%s", SEASONS[season][terrain[x][y]]);
        }
        printf("\n");
    }
}

void printGame(Game* game) {
    printTerrain(game->terrain, game->data.width, game->data.height);
}
