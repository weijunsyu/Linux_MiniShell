#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shellFcn.h"
#include "strFcn.h"
#include "fileFcn.h"

//constants:
#define LINE_LEN 80
#define MAX_ARGS 64
#define MAX_ARG_LEN 64
#define MAX_PATHS 64
#define MAX_PATH_LEN 200
#define WHITESPACE " \t"
#define METACHARACTERS ".|;&<>"

const char* promptString = "minishell >>>";

void printPromt(const char* promptString) {
    printf("%s", promptString );
}

int runShell(char** envp) {
    int ret = 0; //set return as false

    char* line = malloc(LINE_LEN);
    char** args = malloc(MAX_ARGS * sizeof(*args));
    char* path = malloc(MAX_PATH_LEN);
    char** paths = malloc(MAX_PATHS * sizeof(*paths));
    char* cmd = malloc(MAX_PATH_LEN);

    //prompt user
    printPromt(promptString);
    //get argument array from user input
    getArgs(line, args, WHITESPACE);
    //do the command given the arguments
    ret = doCmdLine(path, paths, cmd, args, envp);

    //free memory
    free(line);
    free(args);
    free(path);
    free(paths);
    free(cmd);

    return ret;
}


void getArgs(char* line, char** args, char* delimiters) {
    //get user input
    gets(line);
    //parse line
    int numArgs = 0;
    parse(args, line, delimiters, &numArgs);
}

int doCmdLine(char* path, char** paths, char* cmd, char** args, char** envp) {
    int ret;

    ret = doBuiltIn(args);
    if (ret == -1) {//not builtin
        ret = runCmdLine(path, paths, cmd, args, envp); //if successful return 1 to continue loop
    }//else, cmd was built-in AND was executed during doBuiltIn fcn call
    return ret;

}

 int doBuiltIn(char** args) {
     char* cmd = args[0];
     //check for exit built-in
     if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
         return 0;
     }
     //builtin that do not affect the loop like exit return 1

     return -1; //return -1 to indicate cmd NOT builtin
 }

//Run the command line by splitting into individual command partitions:
int runCmdLine(char* path, char** paths, char* cmd, char** args, char** envp) {
    int flag = 1;
    int current = 0; //index of current arg
    char meta;

    int i;
    for(i = 0; args[i] != NULL; i++) {
        if (sharesFromStr(args[i], METACHARACTERS) != -1) {
            //there is a meta char at index i
            meta = *(args[i]); //get meta char
            if (meta == ';') {
                //run current block as one argument
                args[i] = NULL; //terminate string
                flag = runCmdSegment(path, paths, cmd, &(args[current]), envp, 0);
                current = i+1; //move current to start of next block
            }
            else if (meta == '&') {
                args[i] = NULL;
                //run in background
                flag = runCmdSegment(path, paths, cmd, &(args[current]), envp, 1);
                current = i+1; //move current to start of next block
            }
        }
        if (args[i+1] == NULL && args[current] != NULL) {
            //only one arg left
            flag = runCmdSegment(path, paths, cmd, &(args[current]), envp, 0);
        }
    }
    return flag;
}

int runCmdSegment(char* path, char** paths, char* cmd, char** args, char** envp, int mode) {
    // split up into 2 command chunks and execute in that order:
    // (thing1 <meta> thing2) = thing3    then continue:
    // (thing3 <meta> thing4) = thing5 ... and so forth.
    return runCmd(path, paths, cmd, args, envp, mode); //debugging

}


int runCmd(char* path, char** paths, char* cmd, char** args, char** envp, int mode) {
    int cmdExists = getCmdPath(path, paths, cmd, args); //find command paths

    if (cmdExists) { //cmd exists
        executeCmd(cmd, args, envp, mode);
        return 1;

    }
    else {
        printf("Cmd does not exist!\n");
        return 0;
    }
}

int getCmdPath(char* path, char** paths, char* cmd, char** args) {
    /* get the env var $PATH: and copy it into path. */
    char* tmp = getenv("PATH");
    ////path = realloc(path, (strlen(tmp) + 2));
    strcpy(path, tmp);
    //split up path into individual paths
    int numPaths;
    parse(paths, path, ":", &numPaths);

    //get the fullpath of the command
    return checkFileExistances(cmd, paths, numPaths, args[0]);
}

void executeCmd(char* cmd, char** args, char** envp, int mode) {
    int child_pid;
    int status;
    if (mode == 0) { //fg
        child_pid = fork();
        if (child_pid == 0) { //child process
            execve(cmd, args, envp);
        }
        else {
            waitpid(child_pid, &status, NULL);
        }
    }
    else if (mode == 1) { //bg
        child_pid = fork();
        if (child_pid == 0) { //child process
            execve(cmd, args, envp);
        }
        else {
            return;
        }
    }
}


/*
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//OLD RunCmdLine
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------

//check for metacharacters and multiple commands
int cmdFlag = 0; //flag to indicate cmd termination (0 = not yet at end of cmd, 1 = cmd has ended)
int prevCmdIndex = 0;
char meta;
int fd[2]; //pipe
int fid_in; // input file
int fid_out; //output file

int i;

for (i = 0; args[i] != NULL; i++) {
    if (sharesFromStr(args[i], METACHARACTERS) != -1) {
        //there is a meta char at index i
        meta = *(args[i]); //get meta char (before its set to NULL)
        args[i] = NULL; //set a NULL terminating ptr
        if (meta == ';' || meta == '&') { //TODO linking does not work.
            if (cmdFlag == 1) { //cmd prior has been executed
                cmdFlag = 0; //reset flag
            }
            else { //cmd prior has not been executed
                runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, meta);
            }
        }
        //TODO does not work  currently
        else if (meta == '<') { //file redir input
            fid_in = open(args[++i], O_RDONLY);
            int stdin_copy = dup(0); //copy stdin
            close(0); //close stdin
            dup(fid_in); //open new input
            if (*(args[i + 1]) == '>') { //also redir output
                i += 2; //move to second file
                fid_out = open(args[i], O_WRONLY | O_CREAT);
                int stdout_copy = dup(1); //copy stdout
                close(1); //close stdout
                dup(fid_out); //open new output
                runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, 0);
                close(fid_out); //close output
                dup2(stdout_copy, 1); //restore stdout
            }
            else {
                runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, 0);
            }
            close(fid_in); //close input
            dup2(stdin_copy, 0); //restore stdin
            cmdFlag = 1; //set flag
        }
        else if (meta == '>') {
            fid_out = open(args[++i], O_WRONLY | O_CREAT);
            int stdout_copy = dup(1);
            close(1); //close stdout
            dup(fid_out);
            if (*(args[i + 1]) == '<') {
                i += 2; //move to second file
                fid_in = open(args[i], O_RDONLY);
                int stdin_copy = dup(0);
                close(0);
                dup(fid_in);
                runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, 0);
                close(fid_in);
                dup2(stdin_copy, 0);
            }
            else {
                runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, 0);
            }
            runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, 0);

            close(fid_out);
            dup2(stdout_copy, 1);
            cmdFlag = 1;
        }
        //similar to file redir BUT replace files with the pipe
        else if (meta == '|') { //pipe
            pipe(fd);
            int nextIsPipe = 1;
            int stdin_copy = dup(0);
            int stdout_copy = dup(1);
            int j = i + 1;
            int nextCmdIndex = j;

            close(1); //close stdout
            dup(fd[1]); //switch to pipe output
            runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, 0);

            while(nextIsPipe) {
                if (sharesFromStr(args[j], METACHARACTERS) != -1) {
                    char nMeta = *(args[j]);
                    args[j] = NULL;
                    if (nMeta != '|') { //prev cmd is the tail
                        close(fd[1]); //close pipe output
                        dup2(stdout_copy, 1); //restore stdout
                        close(0); //close stdin
                        dup(fd[0]); //switch to pipe input
                        runCmd(path, paths, cmd, &(args[nextCmdIndex]), envp, nMeta);
                        close(fd[0]); //close pipe input
                        dup2(stdin_copy, 0); //restore stdin
                        nextIsPipe = 0;
                    }
                    else { //prev cmd is part of pipe body in the chain
                        close(0); //close stdin
                        dup(fd[0]); //switch to pipe input
                        runCmd(path, paths, cmd, &(args[nextCmdIndex]), envp, 0);
                    }
                    nextCmdIndex = j + 1;
                }
                else if (args[j] == NULL) { //reached end of args
                    //prev cmd must be tail
                    close(fd[1]); //close pipe output
                    dup2(stdout_copy, 1); //restore stdout
                    close(0); //close stdin
                    dup(fd[0]); //switch to pipe input
                    runCmd(path, paths, cmd, &(args[nextCmdIndex]), envp, 0);
                    close(fd[0]); //close pipe input
                    dup2(stdin_copy, 0); //restore stdin
                    nextIsPipe = 0;
                }
                j++;
            }
            //i = j + 1;

        }


        else { //meta is NOT pipe or redirect or semicolon
            //do nothing
        }
        prevCmdIndex = i + 1;
    }
}

if (args[prevCmdIndex] != NULL) { //leftover cmd exits or no meta in line
    runCmd(path, paths, cmd, &(args[prevCmdIndex]), envp, 0);
}

return 1;
*/
