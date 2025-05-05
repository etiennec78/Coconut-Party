#ifndef MENUS_H
#define MENUS_H

// MARK: - Constants
#define ITEMS_CHAR 20

#define MAIN_ITEMS 4

// MARK: - Structures

// MARK: - Functions
void mainMenuDrawer(char menuItems[][ITEMS_CHAR], int* activeMenu, char* pressedKey);
int mainMenu();

void optionsMenu();

#endif