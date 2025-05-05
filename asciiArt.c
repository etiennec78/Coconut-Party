#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asciiArt.h"

// MARK: - Drawing ascii art passed in argument
void asciiArtDrawer(char* beginMarker, char* endMarker) {
    FILE* asciiArtFile = NULL;
    char actualCaract, search[256];
    int beginMarkerFound;
    
    asciiArtFile = fopen("./resources/ascii-art.txt", "r");
    if (asciiArtFile == NULL){
        printf("🚨 Failed to open file 'ascii-art.txt'\n");
        exit(1);
    }

    while(fgets(search, sizeof(search), asciiArtFile)) {
        if(!beginMarkerFound && strstr(search, beginMarker)) { // NOTE: Research begin marker of the ascii art resqueted
            beginMarkerFound = 1;
            continue;
        }

        if(beginMarkerFound) {
            if(strstr(search, endMarker)) { // NOTE: Stop to print file line/ascii art line when read ended marker
                break;
            }
            
            printf("%s", search);
        }
    }

    printf("\n");
    fclose(asciiArtFile);
}

// MARK: - Format asciiArtDrawer arguments
void asciiArt(char* name) {
    // NOTE: Create the beginning marker of the ascii art
    char* beginMarker = malloc((strlen(name) + 8) * sizeof(char));
    strcpy(beginMarker, "#");
    strcat(beginMarker, name);
    strcat(beginMarker, "~BEGIN");

    // NOTE: Create the ended marker of the ascii art
    char* endMarker = malloc((strlen(name) + 6) * sizeof(char));
    strcpy(endMarker, "#");
    strcat(endMarker, name);
    strcat(endMarker, "~END");

    asciiArtDrawer(beginMarker, endMarker);

    free(beginMarker);
    free(endMarker);
}