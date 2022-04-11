CC=gcc
DEBUG_OPTS=-g
OPTIMIZATION_OPTS=-O3
CC_DEFAULT_OPTS=-Wall -Wextra -std=c99 -pedantic $(DEBUG_OPTS) \
	$(OPTIMIZATION_OPTS)
LINKER_OPTS=-lSDL2 -lm

OBJS=main.o

.c.o:
	$(CC) $(CC_DEFAULT_OPTS) -c $<

all: $(OBJS)
	$(CC) *.o -o main $(LINKER_OPTS)

run: all
	./main

clear:
	rm -f *.o main

application.o: application.c solider.c bullet.c
	gcc -c application.c soldier.c bullet.c -I/opt/homebrew/include -lSDL2 -lSDL2_image

bullet.o: bullet.c
	gcc -c bullet.c -I/opt/homebrew/include -lSDL2 -lSDL2_image

soldier.o: soldier.c
	gcc -c soldier.c -I/opt/homebrew/include -lSDL2 -lSDL2_image

main.o: main.c application.c
	gcc -c main.c application.c

start: src/*.o
	gcc -o Main src/*.c -I/opt/homebrew/include -lSDL2 -lSDL2_image