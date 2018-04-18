#ifndef SHELLFCN_H
#define SHELLFCN_H

/* Print the prompt. */
extern void printPromt(char* promptString);

/* Run one iteration of a command returning 1 meaning continue or 0 meaning quit. */
extern int runShell(char** envp);

/* Given some user input parse using some delimiters and get a char** of
the arguments passed ending with NULL ptr. */
extern void getArgs(char* line, char** args, char* delimiters);

/* Given the args perform the commands, returning 1 if continue, 0 for exit. */
extern int doCmdLine(char* path, char** paths, char* cmd, char** args, char** envp);

/* Check if cmd is a built-in and if yes perform it then return 0 for quit, 1
for successful built-in, or -1 if cmd not built-in. */
extern int doBuiltIn(char** args);

/* Run the command line in its entirety returning 1 if successful. */
extern int runCmdLine(char* path, char** paths, char* cmd, char** args, char** envp);

/*Run singular command, given some mode:
    0 = run in foreground
    ; = run in foreground
    & = run in background */
extern void runCmd(char* path, char** paths, char* cmd, char** args, char** envp, char mode);

/* Obtain the full path of the given commmand returning 1 if successful, 0
otherwise. */
extern int getCmdPath(char* path, char** paths, char* cmd, char** args);

/* Execute one singular command given parameters and any special instructions
given by mode. */
extern void executeCmd(char* cmd, char** args, char** envp, int mode);

#endif
