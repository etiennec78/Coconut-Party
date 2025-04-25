#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void printTerrain(char** terrain, int width, int height) {
    int season = 2;
    switch(season){
        case 1:
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    switch (terrain[i][j]) {
                        case 0: 
                            printf("🌿");
                            break;
                        case 1: 
                            printf("🌸"); 
                            break;
                        case 2: 
                            printf("💧"); 
                            break;
                    }
                }
                printf("\n");
            }
            break;

        case 2:
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    switch (terrain[i][j]) {
                        case 0: 
                            printf("🌳");
                            break;
                        case 1: 
                            printf("🌴"); 
                            break;
                        case 2: 
                            printf("🌊"); 
                            break;
                    }
                }
                printf("\n");
            }
            break;

        case 3:
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    switch (terrain[i][j]) {
                        case 0: 
                            printf("🍁");
                            break;
                        case 1: 
                            printf("🌱"); 
                            break;
                        case 2: 
                            printf("💦"); 
                            break;
                    }
                }
                printf("\n");
            }
            break;

        case 4:
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    switch (terrain[i][j]) {
                        case 0: 
                            printf("🌲");
                            break;
                        case 1: 
                            printf("🪵"); 
                            break;
                        case 2: 
                            printf("🧊"); 
                            break;
                    }
                }
                printf("\n");
            }
            break;
    }
}

void printGame(char** terrain, int width, int height) {
    printTerrain(terrain, width, height);
}
