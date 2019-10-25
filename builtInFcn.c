#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h> //graphics library

#include "builtInFcn.h"


void printBox(int length);

void snakeGame() {
    int flag = 1; //player death flag

    while(flag) { //while player is alive:
        /* Create play area */
        int l = 20; //length of the square to be created
        printBox(l); //print the "box" (play area)



        flag = 0; //player has died
    }


}

void printBox(int length) {
    int lineCounter = 1;
    int i;
    for(i=0; i < length; i++) {
        int j;
        if (lineCounter == 1 || lineCounter == length) {
            printf("|");
            for(j=0; j < length; j++) {
                printf("__");
            }
            printf("|");
        }
        else {
            printf("|");
            for(j=0; j < length; j++) {
                printf("  ");
            }
            printf("|");
        }
        printf("\n");
        lineCounter++;
    }
}
