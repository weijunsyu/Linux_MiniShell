all: minishell

minishell.o: minishell.c strFcn.h fileFcn.h shellFcn.h builtInFcn.h
	gcc -c minishell.c -o minishell.o -Wall -pedantic

strFcn.o: strFcn.c strFcn.h
	gcc -c strFcn.c -o strFcn.o -Wall -pedantic

fileFcn.o: fileFcn.c fileFcn.h
	gcc -c fileFcn.c -o fileFcn.o -Wall -pedantic

shellFcn.o: shellFcn.c shellfcn.h fileFcn.h strFcn.h builtInFcn.h
	gcc -c shellFcn.c -o shellFcn.o -Wall -pedantic

builtInFcn.o: builtInFcn.c builtInFcn.h
	gcc -c builtInFcn.c -o builtInFcn.o -Wall -pedantic

minishell: minishell.o strFcn.o fileFcn.o shellFcn.o builtInFcn.o
	gcc minishell.o strFcn.o fileFcn.o shellFcn.o builtInFcn.o -o minishell -Wall -pedantic

clean:
	rm -f minishell minishell.o strFcn.o fileFcn.o shellFcn.o builtInFcn.o

.PHONY: all clean
