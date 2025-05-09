#include <stdio.h>

#include "display.h"
#include "common.h"
#include "crabs.h"
#include "coins.h"

void moveEmojiCursor(Coordinates coord) {
    printf("\033[%d;%dH", coord.y + 1, 2 * (coord.x) + 1);
}

void colorBackground(int color) {
    printf("\033[48;5;%dm", color);
}

void resetColorBackground() {
    printf("\x1b[1;49m");
}

void ringBell() {
    printf("\a");
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

void printCrab(Game* game, Crab crab) {
    moveEmojiCursor(crab.coord);
    colorBackground(CRAB_TYPE_COLORS[game->data.season][crab.type]);
    printf("%s", ENTITIES[CRAB]);
}

void eraseCrab(Game* game, Crab crab) {
    // Don't erase if there is already a crab there
    if (crabsAtCoord(game, crab.coord) > 1) return;

    moveEmojiCursor(crab.coord);

    // If there is a coin under the crab, print the coin
    if (coinsAtCoord(game, crab.coord)) {
        printCoin(game, crab.coord);
    } else {
        printTerrainTile(game, crab.coord);
    }
}

void printCoin(Game* game, Coordinates coord) {
    moveEmojiCursor(coord);
    colorTerrainTile(game, coord);
    printf("%s", ENTITIES[COIN]);
}

void eraseCoin(Game* game, Coin coin) {
    // Don't erase if there is already a coin there
    if (coinsAtCoord(game, coin.coord) > 1) return;

    // Don't erase if there is a crab there
    if (crabsAtCoord(game, coin.coord)) return;

    moveEmojiCursor(coin.coord);
    printTerrainTile(game, coin.coord);
}

void printDamage(Game* game, Coordinates coord, const char* tile, DamageIndicator indicator, int damage) {
    colorBackground(196);
    moveEmojiCursor(coord);
    printf("%s",tile);
    if (game->data.soundEnabled) ringBell();

    colorTerrainTile(game, indicator.coord);
    moveEmojiCursor(indicator.coord);
    printf("%d", damage);
}

void printBackgroundEntity(Game* game, BackgroundEntity entity) {
    moveEmojiCursor(entity.coord);
    colorTerrainTile(game, entity.coord);
    printf("%s", BACKGROUND_ENTITIES[game->data.season][entity.type]);
}
