all: polish32 polish10

polish32: polish.o base32.o bounds.o
	gcc -g polish.o base32.o bounds.o -o polish32

polish10: polish.o base10.o bounds.o
	gcc -g polish.o base10.o bounds.o -o polish10

polish.o: polish.c base.h bounds.h
	gcc -g -Wall -std=c99 -c polish.c

base10.o: base10.c base.h bounds.h
	gcc -g -Wall -std=c99 -c base10.c

base32.o: base32.c base.h bounds.h
	gcc -g -Wall -std=c99 -c base32.c

bounds.o: bounds.c bounds.h
	gcc -g -Wall -std=c99 -c bounds.c

clean:
	rm -f *.o
	rm -f polish10 polish32
	
	
