#ifndef COINS_H
#define COINS_H

#include "common.h"

// MARK: - Constants

// MARK: - Structures

// MARK: - Declarations of functions
void createCoins(Game* game);
Coin constructCoin(Game* game, Coordinates coord);
void appendCoin(Game* game, Coin coin);
int coinsAtCoord(Game* game, Coordinates coord);
void updateCoins(Game* game);
Coordinates interpolate(Coordinates coord1, Coordinates coord2, double step);

#endif
