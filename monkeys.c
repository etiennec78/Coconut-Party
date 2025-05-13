#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "common.h"
#include "terrain.h"
#include "display.h"
#include "coins.h"
#include "monkeys.h"
#include "crabs.h"

void constructMonkeys(Game* game, Monkeys* monkeys) {
    monkeys->tab = malloc(game->data.monkeyAmount * sizeof(Monkey));
    if (monkeys->tab == NULL) {
        printf("Error: Failed to allocate monkeys !\n");
        free(monkeys->tab);
        exit(1);
    }

    monkeys->length = 0;
    monkeys->shop.focusedMenu = SHOP_TYPE;
    monkeys->shop.selectedMonkey = 0;
    monkeys->shop.selectedType = ALPHA;
}

Monkey constructMonkey(Game* game, Coordinates coord) {
    Monkey monkey;

    monkey.coord = coord;
    monkey.nextAttack = 0;
    monkey.type = NOT_PLACED;
    monkey.stats.attackDistance = 1.5;

    return monkey;
}

void updateMonkeyType(Monkey* monkey, MonkeyType type) {
    switch(type){
        case NOT_PLACED:
            break;

        case ALPHA :
            monkey->stats.attack = 12;
            monkey->stats.attackSpeed = 0.25; // DPS 3
            monkey->stats.attackDistance = 1.1;
            monkey->stats.canFreeze = 0;
            break;

        case BALLISTIC :
            monkey->stats.attack = 3;
            monkey->stats.attackSpeed = 1; // DPS 3
            monkey->stats.attackDistance = 5;
            monkey->stats.canFreeze = 0;
            break;

        case PALMSHAKER :
            monkey->stats.attack = 20;
            monkey->stats.attackSpeed = 0.25; // DPS 5 (zone)
            monkey->stats.attackDistance = 2;
            monkey->stats.canFreeze = 0;
            break;

        case HYPERACTIVE :
            monkey->stats.attack = 2;
            monkey->stats.attackSpeed = 4; // DPS 8
            monkey->stats.attackDistance = 2;
            monkey->stats.canFreeze = 0;
            break;

        case STUNNER :
            monkey->stats.attack = 1;
            monkey->stats.attackSpeed = 0.4; // DPS 0.4 (freeze)
            monkey->stats.attackDistance = 1.5;
            monkey->stats.canFreeze = 1;
            break;

        default:
            printf("Error: Invalid monkey type !\n");
            exit(2);
    }

    monkey->nextAttack = 0;
    monkey->type = type;
}

void buyMonkey(Game* game) {
    Monkey* selectedMonkey = &game->monkeys.tab[game->monkeys.shop.selectedMonkey];
    MonkeyType selectedType = game->monkeys.shop.selectedType;
    int price = MONKEY_PRICES[selectedType - 1];

    if (price <= game->score.coins) {
        game->score.coins -= price;
        updateMonkeyType(selectedMonkey, selectedType);
        printMonkey(game, *selectedMonkey);

        char dataString[SCORE_COLUMN_WIDTH];
        sprintf(dataString, "%d", game->score.coins);
        printScore(UI_COINS, dataString, 0);
    }
}

float getCoordinatesDistance(Coordinates coord1, Coordinates coord2) {
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

    game->end.poppedIndex = game->monkeys.length;
}

void InitializeProjectiles(Game* game) {
    game->projectiles.length = 0;
    game->projectiles.tab = NULL;
}

void createProjectile(Game* game, Coordinates monkeyCoord, Coordinates crabCoord, int monkeyIndex) {
    Projectile projectile;

    projectile.state = PROJECTILE_MOVING; 
    projectile.startCoord = monkeyCoord;
    projectile.targetCoord = crabCoord;
    projectile.currentCoord = monkeyCoord;
    projectile.progression = 0.0;
    projectile.monkeyIndex = monkeyIndex;

    if (game->projectiles.length == 0) {
        game->projectiles.tab = malloc(sizeof(Projectile));
        if (game->projectiles.tab == NULL) {
            printf("Error: Failed to allocate projectibles.tab !\n");
            exit(1);
        }
    } else {
        game->projectiles.tab = realloc(game->projectiles.tab,(game->projectiles.length + 1) * sizeof(Projectile));
         if ( game->projectiles.tab == NULL) {
            printf("Error: Failed to allocate projectibles.tab !\n");
            exit(1);
        }
    }

    game->projectiles.tab[game->projectiles.length] = projectile;
    game->projectiles.length++;
}

void createPalmShakerProjectiles(Game* game, Monkey* monkey, int monkeyIndex) {

    int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

    for (int i = 0; i < 8; i++) {
        Coordinates target;
        target.x = monkey->coord.x + dx[i];
        target.y = monkey->coord.y + dy[i];

        createProjectile(game, target, target, monkeyIndex);
    }
}

void updateProjectiles(Game* game) {
    for (int i = 0; i < game->projectiles.length; i++) {
        Projectile* proj = &game->projectiles.tab[i];
        
        if (proj->state != PROJECTILE_MOVING) continue;

        moveEmojiCursor(proj->currentCoord);
        printTerrainTile(game, proj->currentCoord);

        proj->progression += 1.0 / game->data.framerate;
        
        Coordinates nextCoord = interpolate(proj->startCoord, proj->targetCoord, proj->progression);
        proj->currentCoord = nextCoord;

        moveEmojiCursor(proj->currentCoord);
        printf("%s", ENTITIES[2]);

        if (proj->progression >= 1.0) {
            moveEmojiCursor(proj->currentCoord);
            printTerrainTile(game, proj->currentCoord);
            
            for(int j = 0; j < game->crabs.length; j++) {
                Crab* crab = &game->crabs.tab[j];
                if(!crab->dead && coordsEqual(crab->coord, proj->targetCoord)) {
                    crab->stats.health -= game->monkeys.tab[proj->monkeyIndex].stats.attack;
                    
                    crab->damageIndicator.nextTextFade = game->data.framerate / 2;
                    crab->damageIndicator.nextColorFade = game->data.framerate / 10;
                    printDamage(game, crab->coord, ENTITIES[0], crab->damageIndicator, game->monkeys.tab[proj->monkeyIndex].stats.attack);

                    if(crab->stats.health <= 0) {
                        crab->dead = 1;
                        printTerrainTile(game, crab->coord);
                        Coin coin = constructCoin(game, crab->coord);
                        appendCoin(game, coin);
                        game->score.kills++;
                        game->score.remainingCrabs--;
                        printScore(UI_KILLS, game->score.kills);
                        printScore(UI_ALIVE, game->score.remainingCrabs);
                    } else {
                        // Réafficher le crabe s'il n'est pas mort
                        moveEmojiCursor(crab->coord);
                        printf("%s", ENTITIES[0]);
                    }
                }
            }
            proj->state = PROJECTILE_IMPACT;
        }
    }
}

void attackCrabs(Game* game, Crab* crab, Monkey* monkey, int monkeyIndex) {
    createProjectile(game, monkey->coord, crab->coord, monkeyIndex);
    crab->stats.health -= monkey->stats.attack;
    crab->damageIndicator.nextTextFade = game->data.framerate / 2; // 0.5s
    crab->damageIndicator.nextColorFade = game->data.framerate / 10; // 0.1s

    printDamage(game, crab->coord, ENTITIES[CRAB], crab->damageIndicator, monkey->stats.attack);
    
    if (crab->stats.health <= 0) {
        crab->dead = 1;
        if (!isNullCoord(crab->damageIndicator.coord)) {
            printTerrainTile(game, crab->damageIndicator.coord);
        }

        Coin coin = constructCoin(game, crab->coord);
        appendCoin(game, coin);


        game->score.kills++;
        game->score.remainingCrabs--;

        char dataString[SCORE_COLUMN_WIDTH];
        sprintf(dataString, "%d", game->score.kills);
        printScore(UI_KILLS, dataString, 0);
        sprintf(dataString, "%d", game->score.remainingCrabs);
        printScore(UI_ALIVE, dataString, 0);

        if (game->score.remainingCrabs == 0) {
            startWave(game);
        }
    }
}

void updateEndAnimation(Game* game) {
    if (game->end.nextMonkeyPop < 0) {
        // Destroy the monkey
        game->end.poppedIndex--;
        game->monkeys.tab[game->end.poppedIndex].type = NOT_PLACED;
        printTerrainTile(game, game->monkeys.tab[game->end.poppedIndex].coord);

        // Plan the next monkey pop
        game->end.nextMonkeyPop = 4 * game->data.framerate / game->monkeys.length; // Total: 4s
    } else {
        game->end.nextMonkeyPop--;
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
                    if (monkey->type == PALMSHAKER) {
                        createPalmShakerProjectiles(game, monkey, j);
                        monkey->nextAttack = game->data.framerate * 2;
                        break;  // Important: sortir après avoir lancé l'attaque
                    }

                    if(monkey->stats.canFreeze == 1 && crab->stats.defaultAttackSpeed == crab->stats.attackSpeed && !game->crown.destroyed) {
                        crab->stats.speed = crab->stats.defaultSpeed / 2;
                        crab->stats.attackSpeed = crab->stats.defaultAttackSpeed / 2;
                        crab->nextUnfreeze = 2 * game->data.framerate;
                    }
                    
                    attackCrabs(game, crab, monkey, j);
                    monkey->nextAttack = game->data.framerate / monkey->stats.attackSpeed;
                    break;  // Important: ne cibler qu'un seul crabe
                }
            }
        } else {
            monkey->nextAttack--;
        }
    }

    if (game->crown.destroyed) {
        updateEndAnimation(game);
    }
}
