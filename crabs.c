#include <stdlib.h>

#include "crabs.h"
#include "common.h"

void appendCrab(Game* game, Crab crab) {
    game->crabs.tab[game->crabs.length] = crab;
    game->crabs.length++;
}

Crab constructCrab(Coordinates coord, float defense, float health, float speed, float attack, float attackSpeed) {
    Crab crab;
    crab.pathIndex = 0;
    crab.nextAttack = 0;
    crab.coord = coord;
    crab.stats.defense = defense;
    crab.stats.health = health;
    crab.stats.speed = speed;
    crab.stats.attack = attack;
    crab.stats.attackSpeed = attackSpeed;
    return crab;
}

void createCrabs(Game* game, int amount) {
    game->crabs.length = 0;
    game->crabs.remaining = 0;
    game->crabs.awaitingSpawn = 0;
    game->crabs.nextSpawn = 0;
    game->crabs.tab = malloc(sizeof(Crab) * game->path.length);
}

void attackCrown(Game* game, Crab crab) {
    game->crown.health -= crab.stats.attack;
}

void moveCrabs(Game* game) {
    Crab* crab;
    int pathIndex;
    int nextPathIndex;
    int flooredSpeed;

    // Move all crabs by one step
    for (int i = 0; i < game->crabs.length; i++) {
        crab = &game->crabs.tab[i];
        crab->pathIndex += crab->stats.speed / game->data.framerate;

        if (crab->pathIndex > game->path.length - 2) {
            crab->pathIndex = game->path.length - 2;
        }

        int flooredIndex = crab->pathIndex;
        if (flooredIndex == game->path.length - 2) {
            if (crab->nextAttack <= 0) {
                attackCrown(game, *crab);
                crab->nextAttack = game->data.framerate / crab->stats.attackSpeed;
            } else {
                crab->nextAttack--;
            }
        }

        crab->coord = game->path.tab[flooredIndex];
    }

    // Spawn new crabs untill they have reached the wave limit
    if (game->crabs.awaitingSpawn > 0) {
        if (game->crabs.nextSpawn <= 0) {
            float speed = (100.0 + rand() % 251) / 100; // Range: 1-3.5
            Crab crab = constructCrab(game->path.tab[0], 1, 1, speed, 5, 0.1);
            appendCrab(game, crab);
            game->crabs.awaitingSpawn--;
            game->crabs.nextSpawn = game->data.framerate;
        } else {
            game->crabs.nextSpawn--;
        }
    }
}
