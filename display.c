#include <stdio.h>

#include "display.h"
#include "common.h"

void moveEmojiCursor(Coordinates coord) {
    printf("\033[%d;%dH", coord.y + 1, 2 * (coord.x) + 1);
}

void hideCursor() {
    printf("\033[?25l");
}

void showCursor() {
    printf("\033[?25h");
}

void printTerrainTile(Game* game, Coordinates coord) {
    int season = 1;
    printf("%s", SEASONS[season][game->terrain[coord.x][coord.y]]);
}

void printTerrain(char** terrain, int width, int height) {
    int season = 1;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%s", SEASONS[season][terrain[x][y]]);
        }
        printf("\n");
    }
}

void printCrabs(Game* game) {
    Crab crab;
    for (int i = 0; i < game->crabs.length; i++) {
        crab = game->crabs.tab[i];
        if (crab.stats.health > 0) {
            moveEmojiCursor(crab.coord);
            printf("%s", ENTITIES[0]);
        }
    }
}
