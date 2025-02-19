CFLAGS = -Wall
CC = gcc
# LDFLAGS = -lm (poderia ser necess´aria em outro projeto que utilizasse a math.h)

OBJDIR = objects
BINDIR = bin
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

all: binfolder objfolder bin/main

bin/main: $(OBJ)
	@ echo "Compilando os arquivos objetos no execut´avel"
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

binfolder:
	@ echo "Criando pasta dos bin´arios"
	mkdir -p $(BINDIR)

objfolder:
	@ echo "Criando pasta dos objetos"
	mkdir -p $(OBJDIR)

objects/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

objects/%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean:
	rm bin/* objects/*
	rmdir bin objects

