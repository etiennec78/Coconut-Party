#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void printTerrain(char** terrain, int width, int height) {
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            switch (terrain[i][j]) {
                case 0:
                    printf("🌳");
                    break;
                case 1:
                    printf("🌴");
                    break;
            }
        }
        printf("\n");
    }
}

void printGame(char** terrain, int width, int height) {
    printTerrain(terrain, width, height);
}
