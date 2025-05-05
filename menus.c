#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "asciiArt.h"
#include "menus.h"

void mainMenuDrawer(char menuItems[][ITEMS_CHAR], int* activeMenu, char* pressedKey) {
    *pressedKey = '\0';

    // NOTE: List and shape menu items
    for(int i=0; i<4; i++) {
        for(int e=0; e<=(114-ITEMS_CHAR)/(MAIN_ITEMS+2); e++) {
            printf(" ");
        }
        if(i == *activeMenu) {
            color("7");
            printf("* ");
        }
        printf("%s", menuItems[i]);
        color("0");
    }
    
    *pressedKey = getchar();
    if(*pressedKey == '\033') {
        getchar(); // NOTE: Skip [ char
        switch(getchar()) {
            case 'C': // NOTE: Arrow right
                if(*activeMenu < MAIN_ITEMS-1) {
                    (*activeMenu)++;
                }
                clearLine();
                mainMenuDrawer(menuItems, activeMenu, pressedKey);
                break;
            case 'D': // NOTE: Arrow left
                if(*activeMenu > 0) {
                    (*activeMenu)--;
                }
                clearLine();
                mainMenuDrawer(menuItems, activeMenu, pressedKey);
                break;
        }
    } else if(*pressedKey == '\33') {
        *activeMenu = MAIN_ITEMS-1;
    } else if(*pressedKey != '\r' && *pressedKey != '\n'){
        clearLine();
        mainMenuDrawer(menuItems, activeMenu, pressedKey);
    }
}
int mainMenu() {
    int activeMenu = 0;
    char pressedKey;
    char menuItems[MAIN_ITEMS][ITEMS_CHAR] = { "New game", "Restore game" , "Options", "Exit" }; // Array of MAIN_ITEMS strings with ITEMS_CHAR char max.

    clear();
    asciiArt("coconutParty");
    setRawMode(1); // NOTE: Enable row mode

    mainMenuDrawer(menuItems, &activeMenu, &pressedKey);

    setRawMode(0); // NOTE: Restore canonique mode

    return activeMenu;
}

// TODO: Create menu
void optionsMenu() {
    int optionsSelected, success = 1;
    char menuItems[MAIN_ITEMS][ITEMS_CHAR] = { "", "" , "", "Back" }; // Array of MAIN_ITEMS strings with ITEMS_CHAR char max.

    clear();
    asciiArt("options");
}