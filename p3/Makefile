replace: line.o expand.o replace.c
	gcc -Wall -std=c99 -g line.c expand.c replace.c -o replace

line.o: line.c line.h
	gcc -g -Wall -std=c99 -c line.c

expand.o: expand.c expand.h
	gcc -g -Wall -std=c99 -c expand.c

clean:
	rm -f *.o
	rm -f replace
