#include <stdlib.h>

#include "common.h"
#include "terrain.h"

Path generateMonkeySlots(Game* game) {
    int spacing = game->path.length / game->data.slotAmount;
    int x, y;
    Path monkeySlots;
    constructPath(game, &monkeySlots);
    Coordinates* surroundingTiles;

    for(int i = 0; i <= game->path.length; i += spacing){

        // Get surrounding tiles for each path tile chosen
        int tilesLength = 0;
        surroundingTiles = getSurroundingTiles(game, game->path.tab[i], &tilesLength);
        shuffleCoords(surroundingTiles, tilesLength);

        // Find a surrounding tile not in path, water or already taken
        for(int j = 0; j < tilesLength; j++){
            x = surroundingTiles[j].x;
            y = surroundingTiles[j].y;
            if(!coordsInPath(surroundingTiles[j], game->path) && game->terrain[x][y] != 2 && !coordsInPath(surroundingTiles[j], monkeySlots)){
                monkeySlots.tab[monkeySlots.length] = surroundingTiles[j];
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

