#include "shellfcn.h"

int main(int argc, char *argv[], char* envp[]) {
    int flag = 1;
    while (flag) {
        flag = runShell(envp);
    }
    return 0;
}
