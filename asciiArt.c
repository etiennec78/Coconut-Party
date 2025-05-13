#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asciiArt.h"

void setLineLength(char* line, int* asciiArtWidth) {
    size_t length = strlen(line);

    // Remove all trailing new line characters
    while(length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) {
        line[length - 1] = '\0';
        length--;
    }
            
    if(length > *asciiArtWidth) {
        *asciiArtWidth = length;
    }
}

// MARK: - Drawing ascii art passed as argument
int asciiArtDrawer(char* beginMarker, char* endMarker) {
    FILE* asciiArtFile = NULL;
    char line[256];
    int asciiArtWidth = 0, beginMarkerFound = 0;
    
    asciiArtFile = fopen("./resources/ascii-art.txt", "r");
    if (asciiArtFile == NULL) {
        printf("🚨 Failed to open file 'ascii-art.txt'\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), asciiArtFile)) {

        // NOTE: Research begin marker of the ascii art resqueted
        if (!beginMarkerFound && strstr(line, beginMarker)) {
            beginMarkerFound = 1;
            continue;
        }

        if (beginMarkerFound) {

            // NOTE: Stop printing ascii art when read encouters end marker
            if (strstr(line, endMarker)) {
                break;
            }
            
            printf("%s", line);
            setLineLength(line, &asciiArtWidth);
        }
    }

    printf("\n");
    fclose(asciiArtFile);
    return asciiArtWidth - 1;
}

// MARK: - Format asciiArtDrawer arguments
int asciiArt(const char* name) {

    // NOTE: Create the beginning marker of the ascii art
    char beginMarker[strlen(name) + 8];

    // NOTE: Create the end marker of the ascii art
    char endMarker[strlen(name) + 6];

    snprintf(beginMarker, sizeof(beginMarker), "#%s~BEGIN", name);
    snprintf(endMarker, sizeof(endMarker), "#%s~END", name);

    return asciiArtDrawer(beginMarker, endMarker);
}
