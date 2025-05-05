#ifndef COMMON_H
#define COMMON_H

// MARK: - Constants
#define clear() printf("\033[H\033[2J") // NOTE: Clear screen
#define clearLine() printf("\x1b[2K\r") // NOTE: Clear line
#define color(param) printf("\033[%sm", param)

// MARK: - Structures
typedef struct{
    int x;
    int y;
} Coordinates;

typedef struct {
    Coordinates* tab;
    int length;
} Path;

typedef struct {
    int width;
    int height;
    int endHeight;
    unsigned int seed;
    int minPathLength;
    int maxPathLength;
} Data;

typedef struct {
    char** terrain;
    Path path;
    Data data;
} Game;

// MARK: - Functions
void emptyBuffer();
void setRawMode(int enable);

#endif

/*
    0   Reset               1  Bold
    5   Flashing            7  reverse

                        Black   Red     Green   Yellow  Blue    magenta     cyan    White
    Characteres color   30      31      32      33      34      35          36      37 
    Background color    40      41      42      43      44      45          46      47 
*/