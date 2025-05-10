#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asciiArt.h"

// MARK: - Drawing ascii art passed in argument
void asciiArtDrawer(char* beginMarker, char* endMarker) {
    FILE* asciiArtFile = NULL;
    char search[256];
    int beginMarkerFound = 0;
    
    asciiArtFile = fopen("./resources/ascii-art.txt", "r");
    if(asciiArtFile == NULL) {
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
void asciiArt(const char* name) {
    // NOTE: Create the beginning marker of the ascii art
    int beginMarkerLen = (strlen(name) + 8) * sizeof(char);
    char* beginMarker = malloc((strlen(name) + 8) * sizeof(char));

    // NOTE: Create the ended marker of the ascii art
    int endMarkerLen = (strlen(name) + 6) * sizeof(char);
    char* endMarker = malloc(endMarkerLen);

    if (beginMarker == NULL || endMarker == NULL) {
        printf("🚨 An error occurred during the memory allocation in asciiArt.c !\n");
        exit(1);
    }

    snprintf(beginMarker, beginMarkerLen, "#%s~BEGIN", name);
    snprintf(endMarker, endMarkerLen, "#%s~END", name);

    asciiArtDrawer(beginMarker, endMarker);

    free(beginMarker);
    free(endMarker);
}