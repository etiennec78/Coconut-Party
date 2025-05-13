#include <stdio.h>
#include <string.h>
#include <math.h>

#include "display.h"
#include "common.h"
#include "crabs.h"
#include "coins.h"
#include "monkeys.h"
#include "terrain.h"

const char MONKEY_NAMES[5][12] = {"Alpha", "Ballistic", "Palmshaker", "Hyperactive", "Stunner"};

void moveCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void clear() {
    printf("\033[H\033[2J");
}

void clearLine() {
    printf("\x1b[2K\r");
}

void moveCursorUp(int lines) {
    printf("\x1b[%dA", lines);
}

void moveEmojiCursor(Coordinates coord) {
    moveCursor(3 + SCORE_COLUMN_WIDTH + 2 * (coord.x), 2 + coord.y);
}

void colorBackground(int color) {
    printf("\033[48;5;%dm", color);
}

void blink(int enable) {
    if (enable) {
        printf("\033[5m");
    } else {
        printf("\033[25m");
    }
}

void invertColors() {
    printf("\033[7m");
}

void resetStyle() {
    printf("\033[0m");
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
        resetStyle();
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
    for (UIElement i = UI_CROWN_HEALTH; i <= UI_SHOP; i++) {
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

void printMonkeyShop(Game* game) {
    int selectedIndex = game->monkeys.shop.selectedMonkey;
    int indexLength = countDigits(selectedIndex);
    Monkey monkey = game->monkeys.tab[selectedIndex];
    MonkeyType selectedType = game->monkeys.shop.selectedType;
    int price = MONKEY_PRICES[selectedType - 1];
    const char* typeTitle = MONKEY_NAMES[selectedType - 1];
    char typeSelector[5 + strlen(typeTitle)];
    snprintf(typeSelector, sizeof(typeSelector), "%s%s%s", "< ", typeTitle, " >");

    eraseScore(UI_SHOP, 3);

    // Print selected monkey type
    if (game->monkeys.shop.focusedMenu == SHOP_TYPE) {
        invertColors();
    }
    moveCursor(2 + (SCORE_COLUMN_WIDTH - strlen(typeSelector)) / 2, 4 + UI_SHOP * 3);
    printf("%s", typeSelector);
    resetStyle();

    // Print selected monkey index
    if (game->monkeys.shop.focusedMenu == SHOP_MONKEY) {
        invertColors();
    }
    moveCursor(2 + (SCORE_COLUMN_WIDTH - strlen(SHOP_MONKEY_NB) + indexLength) / 2, 5 + UI_SHOP * 3);
    printf(SHOP_MONKEY_NB, selectedIndex + 1);
    resetStyle();

    // Print buy button
    char buyButtonText[SCORE_COLUMN_WIDTH];
    if (monkey.type == NOT_PLACED) {
        snprintf(buyButtonText, SCORE_COLUMN_WIDTH, "%s%d%s", "BUY (", price, "$)");

        if (price > game->score.coins) {
            colorBackground(196); // Red
        }
    } else {
        strcpy(buyButtonText, "BOUGHT");
    }

    if (game->monkeys.shop.focusedMenu == SHOP_BUY) {
        invertColors();
    }
    moveCursor(2 + (SCORE_COLUMN_WIDTH - strlen(buyButtonText)) / 2, 6 + UI_SHOP * 3);
    printf("%s", buyButtonText);
    resetStyle();
}

void printScore(UIElement element, char* data, int selected) {
    eraseScore(element, 1);
    resetStyle();
    if (selected) invertColors();
    moveCursor(2 + ((SCORE_COLUMN_WIDTH - 3) - strlen(data)) / 2, 4 + element * 3);

    printf("%s", data);
    if (selected) resetStyle();
}

void eraseScore(UIElement element, int lines) {
    resetStyle();
    for (int l = 0; l < lines; l++) {
        moveCursor(2, 4 + element * 3 + l);
        for (int i = 0; i < SCORE_COLUMN_WIDTH; i++) {
            printf(" ");
        }
    }
}

void refreshScores(Game* game) {
    int data[] = {game->crown.health, game->score.coins, game->score.kills, game->score.remainingCrabs, game->score.wave};
    char dataString[SCORE_COLUMN_WIDTH];

    for (int i = UI_CROWN_HEALTH; i < UI_WAVE; i++) {
        sprintf(dataString, "%d", data[i]);
        printScore(i, dataString, 0);
    }

    printMonkeyShop(game);
}

void printWaveShop(Game* game) {
    int selected = game->monkeys.shop.focusedMenu == SHOP_WAVE;
    char dataString[SCORE_COLUMN_WIDTH];
    int bonus;

    if (game->crabs.nextWave <= 0) {
        bonus = 7.5 * 1 / sqrt(game->score.wave);
        sprintf(dataString, "n°%d (+%d$)", game->score.wave, bonus);
    } else {
        bonus = (game->crabs.nextWave / 2);
        sprintf(dataString, "%.1fs (+%d$)", game->crabs.nextWave, bonus);
    }

    printScore(UI_WAVE, dataString, selected);
    resetStyle();
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
        resetStyle();
    }
    printf("%s", ENTITIES[COIN]);
}

void eraseCoin(Game* game, Coin coin) {

    // Don't erase if there is already a coin there
    if (coinsAtCoord(game, coin.coord) > 1) return;

    // Replace old coin by UI or terrain
    if (coordsInTerrain(game, coin.coord)) {

        // Replace by a crab or monkey if there is one under the coin removed
        int crabIndex = getCrabIndexAtCoordinates(game, coin.coord);
        if (crabIndex != -1) {
            printCrab(game, game->crabs.tab[crabIndex]);
        } else {
            int monkeyIndex = getMonkeyIndexAtCoordinates(game->monkeys, coin.coord);
            if (monkeyIndex != -1) {
                printMonkey(game, game->monkeys.tab[monkeyIndex]);
            } else {

                // Replace the coin by terrain
                moveEmojiCursor(coin.coord);
                printTerrainTile(game, coin.coord);
            }
        }
    } else {
        moveEmojiCursor(coin.coord);
        resetStyle();
        printUIAtEmojiCoord(game, coin.coord);
    }
}

void printDamage(Game* game, Coordinates coord, const char* tile, DamageIndicator indicator, int damage) {
    colorBackground(196);
    moveEmojiCursor(coord);
    printf("%s",tile);
    if (game->data.soundEnabled) ringBell();

    if (!isNullCoord(indicator.coord)) {
        colorTerrainTile(game, indicator.coord);
        moveEmojiCursor(indicator.coord);
        printf("%d", damage);
    }
}

void printHeal(Game* game, Crab* crab, const char* tile, int heal) {
    moveEmojiCursor(crab->coord);
    colorBackground(46);  
    printf("%s", tile);
    colorTerrainTile(game, crab->coord);
}

void printMonkey(Game* game, Monkey monkey) {
    moveEmojiCursor(monkey.coord);
    colorBackground(MONKEY_TYPE_COLORS[monkey.type]);
    printf("%s", ENTITIES[MONKEY]);
}

void printBackgroundEntity(Game* game, BackgroundEntity entity) {
    moveEmojiCursor(entity.coord);
    colorTerrainTile(game, entity.coord);
    printf("%s", BACKGROUND_ENTITIES[game->data.season][entity.type]);
}
