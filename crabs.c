#include <stdlib.h>

#include "crabs.h"
#include "terrain.h"
#include "common.h"

void appendCrab(Game* game, Crab crab) {
    game->crabs.tab[game->crabs.length] = crab;
    game->crabs.length++;
}

Crab constructCrab(Coordinates coord, float defense, float health, float speed) {
    Crab crab;
    crab.coord = coord;
    crab.stats.defense = defense;
    crab.stats.health = health;
    crab.stats.speed = speed;
    return crab;
}

void createCrabs(Game* game, int amount) {
    game->crabs.length = 0;
    game->crabs.tab = malloc(sizeof(Crab) * game->path.length);
}

void moveCrabs(Game* game) {
    int pathIndex;

    // Move all crabs by one step
    for (int i = 0; i < game->crabs.length; i++) {
        pathIndex = getIndexAtCoordinates(game->path, game->crabs.tab[i].coord);
        if (pathIndex < game->path.length - 2) {
            game->crabs.tab[i].coord = game->path.tab[pathIndex+1];
        }
    }

    // Spawn new crabs untill they have reached the wave limit
    if (game->crabs.remaining > 0) {
        Crab crab = constructCrab(game->path.tab[0], 1, 1, 1);
        appendCrab(game, crab);
        game->crabs.remaining--;
    }
}
