#ifndef BACKGROUND_ENTITIES_H
#define BACKGROUND_ENTITIES_H

#include "common.h"

// MARK: - Constants

// MARK: - Structures


// MARK: - Declarations of functions
void createBackgroundEntities(Game* game);
void addBackgroundEntity(Game* game, Coordinates coord, BackgroundEntityType type);
void updateBackgroundEntities(Game* game);

#endif
