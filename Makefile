CFLAGS= -Wall -ansi -pedantic -g -Wno-unused-result -lm
CC=gcc

#-------------------------------------------------


EP: main.o fwdList.o arc.o digraph.o
	$(CC) main.o fwdList.o arc.o digraph.o -o EP -lm

main.o: main.c fwdList.h arc.h
	$(CC) $(CFLAGS) -c main.c

fwdList.o: fwdList.h fwdList.c arc.h
	$(CC) $(CFLAGS) -c fwdList.c
	
arc.o: arc.h arc.c
	$(CC) $(CFLAGS) -c arc.c 

digraph.o: digraph.h digraph.c fwdList.h
	$(CC) $(CFLAGS) -c digraph.c

clean:
	rm *.o *~