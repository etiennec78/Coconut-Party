#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "common.h"

// MARK: - Skip scanf char
void emptyBuffer() {
    while(getchar() != '\n') {  }
}

// MARK: - Enable/Disable canonique mode
void setRawMode(int enable) {
    static struct termios oldt, newt;

    // FIXHERE: Translate comments and understand function
    if(enable) {
        tcgetattr(STDIN_FILENO, &oldt); // sauvegarder les paramètres actuels
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // désactiver le mode canonique et l’écho
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // appliquer les nouveaux paramètres
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restaurer les anciens paramètres
    }
}