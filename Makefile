CFLAGS= -Wall -ansi -pedantic -g -Wno-unused-result -lm
CC=gcc

#-------------------------------------------------


EP: main.o utils.o fwdList.o cycle.o arc.o digraph.o
	$(CC) main.o utils.o fwdList.o cycle.o arc.o digraph.o -o EP -lm

main.o: main.c utils.c fwdList.h cycle.h arc.h 
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.h utils.c fwdList.h cycle.h arc.h digraph.h
	$(CC) $(CFLAGS) -c utils.c

fwdList.o: fwdList.h fwdList.c arc.h
	$(CC) $(CFLAGS) -c fwdList.c

cycle.o: cycle.h cycle.c arc.h
	$(CC) $(CFLAGS) -c cycle.c
	
arc.o: arc.h arc.c
	$(CC) $(CFLAGS) -c arc.c 

digraph.o: digraph.h digraph.c fwdList.h
	$(CC) $(CFLAGS) -c digraph.c

clean:
	rm *.o *~