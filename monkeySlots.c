#include <stdlib.h>

#include "common.h"
#include "terrain.h"

Path generateMonkeySlots(Game* game){
    game->data.slotAmount = 15;
    int space = game->path.length / game->data.slotAmount;
    Path monkeySlots;
    constructPath(game, &monkeySlots);
    Coordinates* surroundingTiles;

    for(int i = 0; i < game->path.length; i += space){
        int N = 0, x = 0, y = 0;
        surroundingTiles = getSurroundingTiles(game, game->path.tab[i], &N);
        shuffleCoords(surroundingTiles, N);
        for(int p = 0; p < N; p++){
            x = surroundingTiles[p].x;
            y = surroundingTiles[p].y;
            if(!coordsInPath(surroundingTiles[p], game->path) && game->terrain[x][y] != 2 && !coordsInPath(surroundingTiles[p], monkeySlots)){
                monkeySlots.tab[monkeySlots.length] = surroundingTiles[p];
                monkeySlots.length++;
                break;
            }
        }
       
    }
    
    free(surroundingTiles);
    return monkeySlots;
}


void insertMonkeySlots(Game* game){
    for(int i = 0; i < game->data.slotAmount; i++){
        game->terrain[game->monkeySlots.tab[i].x][game->monkeySlots.tab[i].y] = 6;
    }
}

