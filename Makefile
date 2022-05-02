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

start:
	gcc -Iinclude -Llib -o Main src/*.c src/handlers/*.c src/collision/*.c src/sounds/*.c src/network/*.c src/player/*.c src/draw/*.c src/map/*.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer -lSDL2_ttf
