interpret: value.o parse.o syntax.o interpret.c
	gcc -Wall -std=c99 -g value.c syntax.c parse.c interpret.c -o interpret

parse.o: parse.c parse.h
	gcc -Wall -std=c99 -c parse.c

syntax.o: syntax.c syntax.h
	gcc -Wall -std=c99 -c syntax.c

value.o: value.c value.h
	gcc -Wall -std=c99 -c value.c

clean:
	rm -f *.o
	rm -f interpret
