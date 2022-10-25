kiosk: input.o menu.o kiosk.c
	gcc -Wall -std=c99 -g input.c menu.c kiosk.c -o kiosk

input.o: input.c input.h
	gcc -Wall -std=c99 -c input.c

menu.o: menu.c menu.h
	gcc -Wall -std=c99 -c menu.c

clean:
	rm -f *.o
	rm -f kiosk
