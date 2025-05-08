#include <stdlib.h>
#include <stdio.h>

#include "crabs.h"
#include "coins.h"
#include "common.h"
#include "display.h"
#include "terrain.h"

void createCrabs(Game* game) {
    game->crabs.length = 0;
    game->crabs.remaining = 0;
    game->crabs.awaitingSpawn = 0;
    game->crabs.nextSpawn = 0;
    game->crabs.tab = NULL;
}

void appendCrab(Game* game, Crab crab) {
    // Replace a dead crab
    for (int i = 0; i < game->crabs.length; i++) {
        if (game->crabs.tab[i].dead) {
            game->crabs.tab[i] = crab;
            return;
        }
    }

    // If no crab is dead, resize the crab list
    game->crabs.tab = realloc(game->crabs.tab, (game->crabs.length + 1) * sizeof(Crab));

    game->crabs.tab[game->crabs.length] = crab;
    game->crabs.length++;
}

Crab constructCrab(Game* game, Coordinates coord, int type) {
    Crab crab;
    switch(type){
        case NORMAL:
            crab.stats.health = 10;
            crab.stats.defense = 2;
            crab.stats.speed = (150.0 + rand() % 51) / 100; // 1.5 - 2
            crab.stats.attack = 3;
            crab.stats.attackSpeed = 1; // DPS: 3
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0; 
            break; 

        case GIANT:
            crab.stats.health = 50;
            crab.stats.defense = 4;
            crab.stats.speed = (50.0 + rand() % 51) / 100; // 0.5 - 1
            crab.stats.attack = 35;
            crab.stats.attackSpeed = 0.25; // DPS: 8.75
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0;
            break; 

        case HEALER:
            crab.stats.health = 7;
            crab.stats.defense = 1;
            crab.stats.speed = (50.0 + rand() % 51) / 100; // 0.5 - 1
            crab.stats.attack = 1;
            crab.stats.attackSpeed = 0.1; // DPS: 0.1
            crab.stats.canFly = 0;
            crab.stats.canHeal = 1;
            break; 

        case AGILE:
            crab.stats.health = 2;
            crab.stats.defense = 3;
            crab.stats.speed = (400.0 + rand() % 401) / 100; // 4 - 8
            crab.stats.attack = 1;
            crab.stats.attackSpeed = 3; // DPS: 3
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0;
            break; 

        case FLYING:
            crab.stats.health = 3;
            crab.stats.defense = 1;
            crab.stats.speed = (400.0 + rand() % 201) / 100; // 4 - 6
            crab.stats.attack = 2;
            crab.stats.attackSpeed = 2; // DPS: 4
            crab.stats.canFly = 1;
            crab.stats.canHeal = 0;
            break; 

        case TANK:
            crab.stats.health = 20;
            crab.stats.defense = 20;
            crab.stats.speed = (75.0 + rand() % 26) / 100; // 0.75 - 1
            crab.stats.attack = 3;
            crab.stats.attackSpeed = 1; // DPS: 3
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0;
            break;

        default:
            printf("Error: Invalid crab type !\n");
            exit(1);
    }

    crab.type = type;
    crab.dead = 0;
    crab.pathIndex = 0;
    crab.nextAttack = 0;
    crab.nextPath = game->data.framerate / crab.stats.speed;
    crab.coord = coord;

    return crab;
}

int crabsAtCoord(Game* game, Coordinates coord) {
    int amount = 0;
    for (int i = 0; i < game->crabs.length; i++) {

        if (game->crabs.tab[i].dead) continue;

        if (coordsEqual(game->crabs.tab[i].coord, coord)) {
            amount++;
        }
    }
    return amount;
}

void attackCrown(Game* game, Crab crab) {
    game->crown.health -= crab.stats.attack;
    game->crown.damageIndicator.nextTextFade = game->data.framerate / 2; // 0.5s
    game->crown.damageIndicator.nextColorFade = game->data.framerate / 10; // 0.1s

    printCrab(game, crab);
    printDamage(game, game->path.tab[game->path.length - 1], CROWN, game->crown.damageIndicator, crab.stats.attack);
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
            Coin coin = constructCoin(game, crab->coord);
            appendCoin(game, coin);

            crab->dead = 1;

            continue;
        }

        // Attack the crown if the crab is in front
        if (crab->pathIndex >= game->path.length - 2) {
            if (crab->nextAttack <= 0) {
                attackCrown(game, *crab);
                crab->nextAttack = game->data.framerate / crab->stats.attackSpeed;
            } else {
                crab->nextAttack--;
            }
            continue;
        }

        if (crab->nextPath <= 0) {

            crab->stats.health--; // TODO remove

            eraseCrab(game, *crab);

            crab->pathIndex++;

            // Move the crab up
            crab->coord = game->path.tab[crab->pathIndex];
            printCrab(game, *crab);
            crab->nextPath = game->data.framerate / crab->stats.speed;

        } else {
            crab->nextPath--;
        }
    }

    // Spawn new crabs untill they have reached the wave limit
    if (game->crabs.awaitingSpawn > 0) {
        if (game->crabs.nextSpawn <= 0) {
            CrabType type = rand() % 6;
            Crab crab = constructCrab(game, game->path.tab[0], type);
            appendCrab(game, crab);
            game->crabs.awaitingSpawn--;
            game->crabs.nextSpawn = game->data.framerate / crab.stats.speed;
        } else {
            game->crabs.nextSpawn--;
        }
    }
}
