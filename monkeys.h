#ifndef MONKEYS_H
#define MONKEYS_H

#include "common.h"

// MARK: - Constants

// MARK: - Structures

// MARK: - Declarations of functions
Monkeys generateMonkeys(Game* game);
void insertMonkeys(Game* game);
void updateMonkeys(Game* game);
int coordsInMonkeys(Coordinates coord, Monkeys monkeys);
void buyMonkey(Game* game);
int getCoordinatesDistance(Coordinates coord1, Coordinates coord2);
void InitializeProjectiles(Game* game);
void updateProjectiles(Game* game);
int getMonkeyIndexAtCoordinates(Monkeys monkeys, Coordinates coord);
#endif
