#ifndef FILEFCN_H
#define FILEFCN_H

/* Create full path name from dir and file names and copy the complete path
to cmd string. */
extern void finishPathname(char* cmd, char* file, char* dir);

/* go through array of directories dirPaths and return 1 if cmd is found in a
dir or 0 if cmd does not exist in any directory. */
extern int checkFileExistances(char* cmd, char** dirPaths, int numPaths, char* filename);


#endif
