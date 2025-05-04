#include <stdio.h>

#include "display.h"
#include "common.h"

void moveEmojiCursor(Coordinates coord) {
    printf("\033[%d;%dH", coord.y + 1, 2 * (coord.x) + 1);
}

void colorBackground(int color) {
    printf("\033[48;5;%dm", color);
}

void resetColorBackground() {
    printf("\x1b[1;49m");
}

void hideCursor() {
    printf("\033[?25l");
}

void showCursor() {
    printf("\033[?25h");
}

void colorTerrainTile(Game* game, Coordinates tile) {
    colorBackground(TERRAIN_TILE_COLORS[game->data.season][game->terrain[tile.x][tile.y]]);
}

void printTerrainTileEmoji(Game* game, Coordinates tile) {
    printf("%s", TERRAIN_TILES[game->data.season][game->terrain[tile.x][tile.y]]);
}

void printTerrainTile(Game* game, Coordinates tile) {
    moveEmojiCursor(tile);
    colorTerrainTile(game, tile);
    printTerrainTileEmoji(game, tile);
}

void printTerrain(Game* game) {
    for (int y = 0; y < game->data.height; y++) {
        for (int x = 0; x < game->data.width; x++) {
            colorBackground(TERRAIN_TILE_COLORS[game->data.season][game->terrain[x][y]]);
            printf("%s", TERRAIN_TILES[game->data.season][game->terrain[x][y]]);
        }
        resetColorBackground();
        printf("\n");
    }
}

void printCrab(Crab crab) {
    moveEmojiCursor(crab.coord);
    printf("%s", ENTITIES[0]);
}

void printDamage(Game* game, Coordinates coord, TerrainTile tile, DamageIndicator indicator, float damage) {
    colorBackground(196);
    moveEmojiCursor(coord);
    printTerrainTileEmoji(game, coord);

    colorTerrainTile(game, indicator.coord);
    moveEmojiCursor(indicator.coord);
    printf("%d", (int)damage);
}
