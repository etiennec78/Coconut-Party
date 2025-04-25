#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void printTerrain(char** terrain, int width, int height) {
    int season = 2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%s", SEASONS[season][terrain[i][j]]);
        }
        printf("\n");
    }
}

void printGame(char** terrain, int width, int height) {
    printTerrain(terrain, width, height);
}
