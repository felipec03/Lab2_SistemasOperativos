# Makefile Lab1

# compilador
CC = gcc

# flags
CFLAGS = -Wall -g

# archivos base
SRC = cut.c srep.c count.c archivos.c lab2.c

# objetos
OBJ = cut.o srep.o count.o archivos.o lab2.o

# archivos finales ejecutables por consola
EXECUTABLES = cut srep count lab2

# target por defecto
all: $(EXECUTABLES)

# reglas del makefile
cut: cut.o archivos.o
	$(CC) -o $@ $^

srep: srep.o archivos.o
	$(CC) -o $@ $^

count: count.o archivos.o
	$(CC) -o $@ $^

lab2: lab2.o
	$(CC) -o $@ $^

# se compilan los objetos source a objetos
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

# regla adicional para .c sin .h
archivos.o: archivos.c
	$(CC) $(CFLAGS) -c $<

# limpiar en caso de recompilación y órden
clean:
	rm -f $(OBJ) $(EXECUTABLES)
