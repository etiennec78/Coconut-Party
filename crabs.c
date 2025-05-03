#include <stdlib.h>

#include "crabs.h"
#include "common.h"
#include "display.h"
#include "terrain.h"

void appendCrab(Game* game, Crab crab) {
    game->crabs.tab[game->crabs.length] = crab;
    game->crabs.length++;
}

Crab constructCrab(Coordinates coord, float defense, float health, float speed, float attack, float attackSpeed) {
    Crab crab;
    crab.dead = 0;
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

int crabsAtCoord(Game* game, Coordinates coord) {
    int amount = 0;
    for (int i = 0; i < game->crabs.length; i++) {
        if (coordsEqual(game->crabs.tab[i].coord, coord)) {
            amount++;
        }
    }
    return amount;
}

void eraseCrab(Game* game, Crab crab) {
    moveEmojiCursor(crab.coord);
    printTerrainTile(game, crab.coord);
}

void attackCrown(Game* game, Crab crab) {
    game->crown.health -= crab.stats.attack;
}

void updateCrabs(Game* game) {
    Crab* crab;
    int pathIndex;
    int nextPathIndex;
    int flooredSpeed;

    for (int i = 0; i < game->crabs.length; i++) {
        crab = &game->crabs.tab[i];

        // Manage dead crabs
        if (crab->dead) continue;
        if (crab->stats.health <= 0) {
            eraseCrab(game, *crab);
            crab->dead = 1;
            continue;
        }

        int oldFlooredIndex = crab->pathIndex;

        // Move the crab up
        float newIndex = crab->pathIndex + crab->stats.speed / game->data.framerate;

        // Don't allow crabs to go beyond the crown
        if (newIndex > game->path.length - 2) {
            newIndex = game->path.length - 2;
        }

        // Only draw if the crab has moved
        int newFlooredIndex = newIndex;
        if (oldFlooredIndex != newFlooredIndex) {

            // Only erase if this is the only crab on this tile
            if (crabsAtCoord(game, crab->coord) == 1) {
                eraseCrab(game, *crab);
            }
            crab->pathIndex = newIndex;
            crab->coord = game->path.tab[newFlooredIndex];
            printCrab(game, *crab);
        }
        crab->pathIndex = newIndex;

        // Attack the crown if the crab is in front
        if (newFlooredIndex == game->path.length - 2) {
            if (crab->nextAttack <= 0) {
                attackCrown(game, *crab);
                crab->nextAttack = game->data.framerate / crab->stats.attackSpeed;
            } else {
                crab->nextAttack--;
            }
        }
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
