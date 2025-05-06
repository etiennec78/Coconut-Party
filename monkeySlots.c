#include <stdlib.h>

#include "common.h"
#include "terrain.h"
#include "display.h"

int findMonkeyTileIndex(Game* game, Coordinates* surroundingTiles, int tilesLength, Path monkeySlots) {
    int x, y;

    // Find a surrounding tile not in path, water or already taken
    for(int j = 0; j < tilesLength; j++){
        x = surroundingTiles[j].x;
        y = surroundingTiles[j].y;
        if(!coordsInPath(surroundingTiles[j], game->path) && game->terrain[x][y] != 2 && !coordsInPath(surroundingTiles[j], monkeySlots)){
            return j;
        }
    }

    return -1;
}

Path generateMonkeySlots(Game* game) {
    float spacing = (float)game->path.length / game->data.slotAmount;
    int tilesLength;
    Path monkeySlots;
    constructPath(game, &monkeySlots);
    Coordinates* surroundingTiles;
    Coordinates monkeyTile;

    for(int i = 0; i < game->data.slotAmount; i++){
        int pathIndex, belowMin, aboveMax, monkeyIndex;
        int pathIndexAbove = spacing / 2 + (i * spacing);
        int pathIndexBelow = spacing / 2 + i * spacing - 1;
        int j = -1;

        do {
            j++;
            belowMin = pathIndexBelow < 0;
            aboveMax = pathIndexAbove >= game->path.length;

            if (aboveMax && belowMin) {
                break;
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
            Coordinates* surroundingTiles = getSurroundingTiles(game, game->path.tab[pathIndex], &tilesLength);
            shuffleCoords(surroundingTiles, tilesLength);

            monkeyIndex = findMonkeyTileIndex(game, surroundingTiles, tilesLength, monkeySlots);

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

            free(surroundingTiles);

        } while (monkeyIndex == -1);

        monkeySlots.tab[monkeySlots.length] = monkeyTile;
        monkeySlots.length++;
    }

    return monkeySlots;
}


void insertMonkeySlots(Game* game){
    for(int i = 0; i < game->data.slotAmount; i++){
        game->terrain[game->monkeySlots.tab[i].x][game->monkeySlots.tab[i].y] = MONKEY_SLOT;
    }
}

