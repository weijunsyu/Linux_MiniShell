all: minishell

minishell.o: minishell.c strFcn.h fileFcn.h shellFcn.h
	gcc -c minishell.c -o minishell.o -Wall -pedantic

strFcn.o: strFcn.c strFcn.h
	gcc -c strFcn.c -o strFcn.o -Wall -pedantic

fileFcn.o: fileFcn.c fileFcn.h #strFcn.h
	gcc -c fileFcn.c -o fileFcn.o -Wall -pedantic

shellFcn.o: shellFcn.c shellfcn.h fileFcn.h strFcn.h
	gcc -c shellFcn.c -o shellFcn.o -Wall -pedantic

minishell: minishell.o strFcn.o fileFcn.o shellFcn.o
	gcc minishell.o strFcn.o fileFcn.o shellFcn.o -o minishell -Wall -pedantic

clean:
	rm -f minishell minishell.o strFcn.o fileFcn.o shellFcn.o

.PHONY: all clean
