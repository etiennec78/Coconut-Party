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
    printf("%s", TERRAIN_CASES[game->data.season][game->terrain[coord.x][coord.y]]);
}

void printTerrain(Game* game) {
    for (int y = 0; y < game->data.height; y++) {
        for (int x = 0; x < game->data.width; x++) {
            printf("%s", TERRAIN_CASES[game->data.season][game->terrain[x][y]]);
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
