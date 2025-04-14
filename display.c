#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void printTerrain(char** terrain, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (terrain[x][y]) {
                case 0:
                    printf("🌳");
                    break;
                case 1:
                    printf("🌴");
                    break;
                case 2:
                    printf("🌊");
                    break;
                case 3:
                    printf("🟫");
                    break;
                case 4:
                    printf("🏁");
                    break;
                case 5:
                    printf("👑");
                    break;
            }
        }
        printf("\n");
    }
}

void printGame(Game* game) {
    printTerrain(game->terrain, game->data.width, game->data.height);
}
