#include <stdlib.h>

#include "common.h"
#include "display.h"
#include "terrain.h"

void createCoins(Game* game) {
    game->coins.tab = NULL;
    game->coins.tab = 0;
}

Coin constructCoin(Game* game, Coordinates coord) {
    Coin coin;
    coin.active = 1;
    coin.coord = coord;
    coin.nextCollection = 3 * game->data.framerate; // 3s

    printCoin(game, coord);

    return coin;
}

void appendCoin(Game* game, Coin coin) {
    // Replace unactive coins
    for (int i = 0; i < game->coins.length; i++) {
        if (!game->coins.tab[i].active) {
            game->coins.tab[i] = coin;
            return;
        }
    }

    // If no coin is unactive, resize the coins list
    game->coins.tab = realloc(game->coins.tab, (game->coins.length + 1) * sizeof(Coin));

    game->coins.tab[game->coins.length] = coin;
    game->coins.length++;
}

int coinsAtCoord(Game* game, Coordinates coord) {
    int amount = 0;
    for (int i = 0; i < game->coins.length; i++) {

        if (!game->coins.tab[i].active) continue;

        if (coordsEqual(game->coins.tab[i].coord, coord)) {
            amount++;
        }
    }
    return amount;
}

void updateCoins(Game* game) {
    for (int i = 0; i < game->coins.length; i++) {
        Coin* coin = &game->coins.tab[i];

        if (!coin->active) continue;

        if (coin->nextCollection <= 0) {
            coin->active = 0;
            eraseCoin(game, *coin);
        } else {
            coin->nextCollection--;
        }
    }
}
