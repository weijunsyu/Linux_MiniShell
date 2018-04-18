#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "fileFcn.h"
//#include "strFcn.h"

void finishPathname(char* cmd, char* file, char* dir) {
    strcpy(cmd, dir);
    strcat(cmd, "/");
    strcat(cmd, file);
}


int checkFileExistances(char* cmd, char** dirPaths, int numPaths, char* filename) {
    int i;
    for(i = 0; i < numPaths; i++) {
        finishPathname(cmd, filename, dirPaths[i]);
        if (access(cmd, F_OK) != -1) {
            return 1;
        }
    }
    return 0;
}
