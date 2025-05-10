#include <stdio.h>
#include <string.h>

#include "display.h"
#include "common.h"
#include "crabs.h"
#include "coins.h"
#include "terrain.h"

void moveCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void moveEmojiCursor(Coordinates coord) {
    moveCursor(3 + SCORE_COLUMN_WIDTH + 2 * (coord.x), 2 + coord.y);
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

void addToUI(Game* game, UIElement element) {
    int x = 2 + (SCORE_COLUMN_WIDTH - 3 - strlen(UI_ELEMENTS[element])) / 2;
    int y = 3 + element * 3;

    moveCursor(x, y);
    printf("%s", UI_ELEMENTS[element]);

    // Place corresponding emoji 2 steps away from terrain left side
    Coordinates emojiCoord = {-2, y - 2};
    moveEmojiCursor(emojiCoord);
    printf("%s", UI_EMOJIS[element]);

    if (element == UI_COINS) {
        game->coins.scoreCoord = emojiCoord;
    }
}

void printUIAtCoord(Game* game, Coordinates coord) {
    if (coord.x == 1) {
        if (coord.y == 1) {
            printf("╔");
        } else if (coord.y == game->data.height + 2) {
            printf("╚");
        } else {
            printf("║");
        }
    } else if (coord.x == 2 + SCORE_COLUMN_WIDTH) {
        if (coord.y == 1) {
            printf("╦");
        } else if (coord.y == game->data.height + 2) {
            printf("╩");
        } else {
            printf("║");
        }
    } else if (coord.x == 3 + SCORE_COLUMN_WIDTH + game->data.width * 2) {
        if (coord.y == 1) {
            printf("╗");
        } else if (coord.y == game->data.height + 2) {
            printf("╝");
        } else {
            printf("║");
        }
    } else {
        if (coord.y == 1 || coord.y == game->data.height + 2) {
            printf("═");
        } else {
            printf(" ");
        }
    }
}

void printUIAtEmojiCoord(Game* game, Coordinates coord) {
    // Get terminal coordinates from terrain coordinates
    Coordinates globalCoord = terrainCoordToGlobalCoord(coord);

    printUIAtCoord(game, globalCoord);
    globalCoord.x++;
    printUIAtCoord(game, globalCoord);
}

void printTerrain(Game* game) {

    // Top box edge
    printf("╔");
    for (int i = 0; i < SCORE_COLUMN_WIDTH; i++) {
        printf("═");
    }
    printf("╦");
    for (int i = 0; i < game->data.width * 2; i++) {
        printf("═");
    }
    printf("╗\n");

    for (int y = 0; y < game->data.height; y++) {
        // Left box edge
        printf("║");
        for (int i = 0; i < SCORE_COLUMN_WIDTH; i++) {
            printf(" ");
        }
        printf("║");

        // Terrain
        for (int x = 0; x < game->data.width; x++) {
            colorBackground(TERRAIN_TILE_COLORS[game->data.season][game->terrain[x][y]]);
            printf("%s", TERRAIN_TILES[game->data.season][game->terrain[x][y]]);
        }

        // Right box edge
        resetColorBackground();
        printf("║\n");
    }

    // Bottom box edge
    printf("╚");
    for (int i = 0; i < SCORE_COLUMN_WIDTH; i++) {
        printf("═");
    }
    printf("╩");
    for (int i = 0; i < game->data.width * 2; i++) {
        printf("═");
    }
    printf("╝\n");

    // Add UI elements
    for (UIElement i = UI_WAVE; i <= UI_ALIVE; i++) {
        addToUI(game, i);
    }
}

int countDigits(int number) {
    int digits = 0;

    if (number == 0)
        return 1;

    while (number != 0) {
        number /= 10;
        digits++;
    }

    return digits;
}

void printScore(UIElement element, int data) {
    int dataLength = countDigits(data);

    resetColorBackground();
    moveCursor(2 + ((SCORE_COLUMN_WIDTH - 3) - dataLength) / 2, 4 + element * 3);

    printf("%d", data);
}

void refreshScores(Game* game) {
    printScore(UI_WAVE, game->score.wave);
    printScore(UI_COINS, game->score.coins);
    printScore(UI_KILLS, game->score.kills);
    printScore(UI_ALIVE, game->score.remainingCrabs);
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
        printCoinOnMap(game, crab.coord);
    } else {
        printTerrainTile(game, crab.coord);
    }
}

void printCoinOnMap(Game* game, Coordinates coord) {
    moveEmojiCursor(coord);
    colorTerrainTile(game, coord);
    printf("%s", ENTITIES[COIN]);
}

void printCoin(Game* game, Coin coin) {
    moveEmojiCursor(coin.coord);
    if (coordsInTerrain(game, coin.coord)) {
        colorTerrainTile(game, coin.coord);
    } else {
        resetColorBackground();
    }
    printf("%s", ENTITIES[COIN]);
}

void eraseCoin(Game* game, Coin coin) {

    // Don't erase if there is already a coin there
    if (coinsAtCoord(game, coin.coord) > 1) return;

    // Replace old coin by UI or terrain
    if (coordsInTerrain(game, coin.coord)) {

        // Don't erase if there is a crab there
        if (crabsAtCoord(game, coin.coord)) return;

        moveEmojiCursor(coin.coord);
        printTerrainTile(game, coin.coord);

    } else {
        moveEmojiCursor(coin.coord);
        resetColorBackground();
        printUIAtEmojiCoord(game, coin.coord);
    }
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

void printHeal(Game* game, Crab* crab, const char* tile, int heal) {
    moveEmojiCursor(crab->coord);
    colorBackground(46);  
    printf("%s", tile);
    colorTerrainTile(game, crab->coord);
}

void printBackgroundEntity(Game* game, BackgroundEntity entity) {
    moveEmojiCursor(entity.coord);
    colorTerrainTile(game, entity.coord);
    printf("%s", BACKGROUND_ENTITIES[game->data.season][entity.type]);
}
