#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asciiArt.h"

// MARK: - Drawing ascii art passed as argument
void asciiArtDrawer(char* beginMarker, char* endMarker) {
    FILE* asciiArtFile = NULL;
    char search[256];
    int beginMarkerFound = 0;
    
    asciiArtFile = fopen("./resources/ascii-art.txt", "r");
    if (asciiArtFile == NULL) {
        printf("🚨 Failed to open file 'ascii-art.txt'\n");
        exit(1);
    }

    while (fgets(search, sizeof(search), asciiArtFile)) {

        // NOTE: Research begin marker of the ascii art resqueted
        if (!beginMarkerFound && strstr(search, beginMarker)) {
            beginMarkerFound = 1;
            continue;
        }

        if (beginMarkerFound) {

            // NOTE: Stop printing ascii art when read encouters end marker
            if (strstr(search, endMarker)) {
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
    char beginMarker[strlen(name) + 8];

    // NOTE: Create the end marker of the ascii art
    char endMarker[strlen(name) + 6];

    snprintf(beginMarker, sizeof(beginMarker), "#%s~BEGIN", name);
    snprintf(endMarker, sizeof(endMarker), "#%s~END", name);

    asciiArtDrawer(beginMarker, endMarker);
}
