#include <stdlib.h>
#include <stdio.h>

#include "crabs.h"
#include "common.h"
#include "display.h"
#include "terrain.h"

void appendCrab(Game* game, Crab crab) {
    game->crabs.tab[game->crabs.length] = crab;
    game->crabs.length++;
}

Crab constructCrab(Game* game, Coordinates coord, int type) {
    Crab crab;
    
    switch(type){

        case 0 :            
            //NORMAL STATS
            crab.stats.health = 10;
            crab.stats.defense = 1;
            crab.stats.speed = (100.0 + rand() % 251) / 200;
            crab.stats.attack = 5;
            crab.stats.attackSpeed = 1;
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0; 
            break; 

        case 1 :            
            //GIANT STATS
            crab.stats.health = 10;
            crab.stats.defense = 10;
            crab.stats.speed = (100.0 + rand() % 251) / 100;
            crab.stats.attack = 5;
            crab.stats.attackSpeed = 1;
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0;
            break; 

        case 2:
            //HEALER STATS
            crab.stats.health = 7;
            crab.stats.defense = 1;
            crab.stats.speed = (100.0 + rand() % 251) / 100;
            crab.stats.attack = 5;
            crab.stats.attackSpeed = 1;
            crab.stats.canFly = 0;
            crab.stats.canHeal = 1;
            break; 

        case 3 :           
            //AGILE STATS
            crab.stats.health = 10;
            crab.stats.defense = 1;
            crab.stats.speed = (100.0 + rand() % 251) / 50;
            crab.stats.attack = 5;
            crab.stats.attackSpeed = 2;
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0;
            break; 

        case 4 :           
             //FLYING CRABS STATS
            crab.stats.health = 1;
            crab.stats.defense = 1;
            crab.stats.speed = (100.0 + rand() % 251) / 75;
            crab.stats.attack = 5;
            crab.stats.attackSpeed = 2;
            crab.stats.canFly = 1;
            crab.stats.canHeal = 0;
            break; 

        case 5 :          
            //TANK STATS
            crab.stats.health = 40;
            crab.stats.defense = 1;
            crab.stats.speed = (100.0 + rand() % 251) / 200; 
            crab.stats.attack = 5;
            crab.stats.attackSpeed = 1;
            crab.stats.canFly = 0;
            crab.stats.canHeal = 0;
            break; 

        default :
            printf("Error: Invalid crab type !\n");
            exit(1);
    }

    crab.dead = 0;
    crab.pathIndex = 0;
    crab.nextAttack = 0;
    crab.nextPath = game->data.framerate / crab.stats.speed;
    crab.coord = coord;

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
    game->crown.damageIndicator.nextTextFade = game->data.framerate / 2; // 0.5s
    game->crown.damageIndicator.nextColorFade = game->data.framerate / 10; // 0.1s

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
            eraseCrab(game, *crab);
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

            // Only erase if this is the only crab on this tile
            if (crabsAtCoord(game, crab->coord) == 1) {
                eraseCrab(game, *crab);
            }

            crab->pathIndex++;

            // Move the crab up
            crab->coord = game->path.tab[crab->pathIndex];
            printCrab(*crab);
            crab->nextPath = game->data.framerate / crab->stats.speed;

        } else {
            crab->nextPath--;
        }
    }

    // Spawn new crabs untill they have reached the wave limit
    if (game->crabs.awaitingSpawn > 0) {
        if (game->crabs.nextSpawn <= 0) {
            float speed = (100.0 + rand() % 251) / 100; // Range: 1-3.5
            int type = rand() % 6;
            Crab crab = constructCrab(game, game->path.tab[0], type);
            appendCrab(game, crab);
            game->crabs.awaitingSpawn--;
            game->crabs.nextSpawn = game->data.framerate / crab.stats.speed;
        } else {
            game->crabs.nextSpawn--;
        }
    }
}
