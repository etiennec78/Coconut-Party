#ifndef CRABS_H
#define CRABS_H

#include "common.h"

// MARK: - Constants

// MARK: - Structures

// MARK: - Declarations of functions
void createCrabs(Game* game);
void updateCrabs(Game* game);
int crabsAtCoord(Game* game, Coordinates coord);

#endif
