hash: ripeMD.o byteBuffer.o hash.c
	gcc -Wall -std=c99 -g hash.c byteBuffer.c ripeMD.c -o hash

byteBuffer.o: byteBuffer.h byteBuffer.c
	gcc -Wall -std=c99 -c byteBuffer.c

ripeMD.o: ripeMD.c ripeMD.h
	gcc -Wall -std=c99 -c ripeMD.c

testdriver: ripeMD.c byteBuffer.c
	gcc -Wall -std=c99 -g -DTESTABLE testdriver.c ripeMD.c byteBuffer.c -o testdriver

clean:
	rm -f *.o
	rm -f hash
