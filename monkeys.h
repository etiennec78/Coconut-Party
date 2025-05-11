#ifndef MONKEYS_H
#define MONKEYS_H

#include "common.h"

// MARK: - Constants

// MARK: - Structures

// MARK: - Declarations of functions
Monkeys generateMonkeys(Game* game);
void insertMonkeys(Game* game);
void updateMonkeys(Game* game);
int getCoordinatesDistance(Coordinates coord1, Coordinates coord2);
int getMonkeyIndexAtCoordinates(Monkeys monkeys, Coordinates coord);
#endif
