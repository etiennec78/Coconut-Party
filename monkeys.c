#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "common.h"
#include "terrain.h"
#include "display.h"
#include "coins.h"

void constructMonkeys(Game* game, Monkeys* monkeys) {
    monkeys->tab = malloc(game->data.monkeyAmount * sizeof(Monkey));
    if (monkeys->tab == NULL) {
        printf("Error: Failed to allocate monkeys !\n");
        free(monkeys->tab);
        exit(1);
    }

    monkeys->length = 0;
}

Monkey constructMonkey(Game* game, Coordinates coord) {
    Monkey monkey;

    monkey.coord = coord;
    monkey.nextAttack = 0;
    monkey.type = NOT_PLACED;
    monkey.stats.attackDistance=1.5;

    return monkey;
}

void updateMonkeyType(Game* game, Monkey* monkey, MonkeyType type) {
    switch(type){
        case NOT_PLACED:
            break;

        case ALPHA :
            monkey->stats.attack = 5;
            monkey->stats.attackSpeed = 0.25; 
            monkey->stats.attackDistance = 0.5 ;
            monkey->stats.canFreeze = 0;
            break;

        case BALLISTIC :
            monkey->stats.attack = 3;
            monkey->stats.attackSpeed = 1; 
            monkey->stats.attackDistance = 3 ;
            monkey->stats.canFreeze = 0;
            break;

        case PALMSHAKER :
            monkey->stats.attack = 1;
            monkey->stats.attackSpeed = 3; 
            monkey->stats.attackDistance = 1.5 ;
            monkey->stats.canFreeze = 0;
            break;

        case HYPERACTIVE :
            monkey->stats.attack = 3;
            monkey->stats.attackSpeed = 1; 
            monkey->stats.attackDistance = 2 ;
            monkey->stats.canFreeze = 0;
            break;

        case STUNNER :
            monkey->stats.attack = 1;
            monkey->stats.attackSpeed = 0.4;
            monkey->stats.attackDistance = 1.5 ;
            monkey->stats.canFreeze = 1;
            break;

        default:
            printf("Error: Invalid monkey type !\n");
            exit(1);
    }

    monkey->nextAttack = 0;
    monkey->type = type;
}

int getCoordinatesDistance(Coordinates coord1, Coordinates coord2) {
    return sqrt((coord1.x - coord2.x) * (coord1.x - coord2.x) + (coord1.y - coord2.y) * (coord1.y - coord2.y));
}

int getMonkeyIndexAtCoordinates(Monkeys monkeys, Coordinates coord) {
    for (int i = monkeys.length - 1; i >= 0; i--) {
        if (coordsEqual(monkeys.tab[i].coord, coord)) {
            return i;
        }
    }
    return -1;
}

int coordsInMonkeys(Coordinates coord, Monkeys monkeys) {
    return getMonkeyIndexAtCoordinates(monkeys, coord) != -1;
}

int findMonkeyTileIndex(Game* game, Coordinates* surroundingTiles, int tilesLength, Monkeys monkeys) {
    int x, y;

    // Find a surrounding tile not in path, water or already taken
    for(int j = 0; j < tilesLength; j++){
        x = surroundingTiles[j].x;
        y = surroundingTiles[j].y;
        if(!coordsInPath(surroundingTiles[j], game->path) && game->terrain[x][y] != 2 && !coordsInMonkeys(surroundingTiles[j], monkeys)){
            return j;
        }
    }

    return -1;
}

Monkeys generateMonkeys(Game* game) {
    float spacing = (float)game->path.length / game->data.monkeyAmount;
    int tilesLength;
    Monkeys monkeys;
    constructMonkeys(game, &monkeys);
    Coordinates* surroundingTiles;
    Coordinates monkeyTile;

    for (int i = 0; i < game->data.monkeyAmount; i++) {
        int pathIndex, belowMin, aboveMax, monkeyIndex;
        int pathIndexAbove = spacing / 2 + (i * spacing);
        int pathIndexBelow = spacing / 2 + i * spacing - 1;
        int j = -1;

        do {
            j++;
            belowMin = pathIndexBelow < 0;
            aboveMax = pathIndexAbove >= game->path.length;

            // If there are no path tiles available, return monkeys
            if (aboveMax && belowMin) {
                return monkeys;
            }

            if (j % 2 == 0) {
                if (aboveMax) continue;
                pathIndex = pathIndexAbove;
            } else {
                if (belowMin) continue;
                pathIndex = pathIndexBelow;
            }

            // Find a surrounding tile available for a monkey slot
            tilesLength = 0;
            surroundingTiles = getSurroundingTiles(game, game->path.tab[pathIndex], &tilesLength);

            shuffleCoords(surroundingTiles, tilesLength);

            monkeyIndex = findMonkeyTileIndex(game, surroundingTiles, tilesLength, monkeys);

            // If there were no tile available, loop again and scan next tile
            if (monkeyIndex == -1) {
                if (j % 2 == 0) {
                    pathIndexAbove++;
                } else {
                    pathIndexBelow--;
                }
                j++;
            } else {
                monkeyTile = surroundingTiles[monkeyIndex];
            }

            if (surroundingTiles != NULL) {
                free(surroundingTiles);
            }

        } while (monkeyIndex == -1);

        if (monkeyTile.x >= 0 && monkeyTile.x < game->data.width &&
            monkeyTile.y >= 0 && monkeyTile.y < game->data.height) {
            
            Monkey monkey = constructMonkey(game, monkeyTile);
            MonkeyType type = rand()%5+1;
            updateMonkeyType(game, &monkey, type);
            monkeys.tab[monkeys.length] = monkey;
            monkeys.length++;
        }
    }

    return monkeys;
}

void insertMonkeys(Game* game) {
    for (int i = 0; i < game->data.monkeyAmount; i++) {
        int x = game->monkeys.tab[i].coord.x;
        int y = game->monkeys.tab[i].coord.y;

        game->terrain[x][y] = MONKEY_SLOT;
    }
}

void attackCrabs(Game* game, Crab* crab, Monkey* monkey) {

    crab->stats.health -= monkey->stats.attack;
    crab->damageIndicator.nextTextFade = game->data.framerate / 2; // 0.5s
    crab->damageIndicator.nextColorFade = game->data.framerate / 10; // 0.1s

    printDamage(game, crab->coord, ENTITIES[0], crab->damageIndicator, monkey->stats.attack);
    
    if (crab->stats.health <= 0) {
        crab->dead = 1;
        printTerrainTile(game, crab->damageIndicator.coord);

        Coin coin = constructCoin(game, crab->coord);
        appendCoin(game, coin);


        game->score.kills++;
        game->score.remainingCrabs--;
        printScore(UI_KILLS, game->score.kills);
        printScore(UI_ALIVE, game->score.remainingCrabs);
    }
}

void updateMonkeys(Game* game) {
   
    for (int j = 0 ; j< game->monkeys.length ; j ++) {
        Monkey* monkey = &game->monkeys.tab[j];

        if (monkey->type == NOT_PLACED) {
            continue;
        }

        if (monkey->nextAttack <= 0) {

            for(int i = 0 ; i < game->crabs.length ; i++){
                Crab* crab = &game->crabs.tab[i];
    
                if (crab->dead) continue;

                if (getCoordinatesDistance(monkey->coord, crab->coord) < monkey->stats.attackDistance) {

                    if(monkey->stats.canFreeze == 1 && crab->stats.defaultAttackSpeed == crab->stats.attackSpeed) {
                        crab->stats.speed = crab->stats.defaultSpeed / 4;
                        crab->stats.attackSpeed = crab->stats.defaultAttackSpeed / 4;
                        crab->nextUnfreeze = 2 * game->data.framerate;
                    }
                    
                    attackCrabs(game, crab, monkey);
                    monkey->nextAttack = game->data.framerate / monkey->stats.attackSpeed;

                }
            }
        } else {
            monkey->nextAttack--;
        }
    }
}
