#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "display.h"
#include "terrain.h"

void createCoins(Game* game) {
    game->coins.tab = NULL;
    game->coins.length = 0;
}

Coin constructCoin(Game* game, Coordinates coord) {
    Coin coin;
    coin.state = COIN_ON_MAP;
    coin.coord = coord;
    coin.startCoord = coord;
    coin.nextCollection = 3 * game->data.framerate; // 3s
    coin.collectionProgression = 0.0;

    printCoin(game, coin);

    return coin;
}

void appendCoin(Game* game, Coin coin) {
    // Replace disabled coins
    for (int i = 0; i < game->coins.length; i++) {
        if (game->coins.tab[i].state == COIN_DISABLED) {
            game->coins.tab[i] = coin;
            return;
        }
    }

    // If no coin is disabled, resize the coins list
    game->coins.tab = realloc(game->coins.tab, (game->coins.length + 1) * sizeof(Coin));

    game->coins.tab[game->coins.length] = coin;
    game->coins.length++;
}

int coinsAtCoord(Game* game, Coordinates coord) {
    int amount = 0;
    for (int i = 0; i < game->coins.length; i++) {

        if (game->coins.tab[i].state == COIN_DISABLED) continue;

        if (coordsEqual(game->coins.tab[i].coord, coord)) {
            amount++;
        }
    }
    return amount;
}

Coordinates interpolate(Coordinates coord1, Coordinates coord2, double step) {
    Coordinates result;
    result.x = coord1.x + (coord2.x - coord1.x) * step;
    result.y = coord1.y + (coord2.y - coord1.y) * step;
    return result;
}

void updateCoinOnMap(Game* game, Coin* coin) {
    if (coin->nextCollection <= 0) {

        // Enable coin collection
        coin->state = COIN_COLLECTION;
        eraseCoin(game, *coin);

    } else {
        coin->nextCollection--;
    }
}

void updateCoinCollection(Game* game, Coin* coin) {
    coin->collectionProgression += 1.0 / game->data.framerate; // Total: 1s

    // Find next coordinates
    Coordinates nextCoord = interpolate(coin->startCoord, game->coins.scoreCoord, coin->collectionProgression);

    // If the coin hasn't reached his target
    if (coin->collectionProgression < 1) {

        // If the next coordinates have changed
        if (!coordsEqual(nextCoord, coin->coord)) {

            // Place the coin at the new coordinates
            eraseCoin(game, *coin);
            coin->coord = nextCoord;
            printCoin(game, *coin);
        }

    } else {

        // Fix emoji being slightly misplaced because of floating point imprecisions
        eraseCoin(game, *coin);
        coin->coord = game->coins.scoreCoord;
        moveEmojiCursor(coin->coord);

        // Update score
        game->score.coins++;
        printScore(UI_COINS, game->score.coins);

        coin->state = COIN_DISABLED;
    }
}

void updateCoins(Game* game) {
    for (int i = 0; i < game->coins.length; i++) {
        Coin* coin = &game->coins.tab[i];

        switch (coin->state) {
            case COIN_DISABLED:
                continue;

            case COIN_ON_MAP:
                updateCoinOnMap(game, coin);
                break;

            case COIN_COLLECTION:
                updateCoinCollection(game, coin);
                break;
        };
    }
}
