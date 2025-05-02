#include "common.h"
#include <stdio.h>
#include <stdlib.h>

Coordinates* generateMonkeySlots(Game* game){
    game->data.slotAmount = 10;
    int space = game->path.length / game->data.slotAmount;
    Coordinates* monkeySlots = malloc(sizeof(Coordinates)*game->data.slotAmount);
    int j = 0;
    for(int i = 0; i <= game->path.length;i+=space){
        monkeySlots[j] = game->path.tab[i];
        j++;
    }
    

    for(int k = 0; k < 20; k++) {
        printf("chemin %d: (%d, %d)\n", k, game->path.tab[k].x, game->path.tab[k].y); //TEST chemin
    }

    for(int k = 0; k < game->data.slotAmount; k++) {
        printf("Slot %d: (%d, %d)\n", k, monkeySlots[k].x, monkeySlots[k].y); //TEST
    }

   
    // RAJOUTER BIEN QU ILS SONT SUR ARBRE ET PAS CHEMIN


    return monkeySlots;
}


void insertMonkeySlots(Game* game){
    for(int i=0;i<game->data.slotAmount;i++){
        game->terrain[game->monkeySlots[i].x][game->monkeySlots[i].y]=6;
    }
}
