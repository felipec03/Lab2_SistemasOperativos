# Makefile Lab1

# compilador
CC = gcc

# flags
CFLAGS = -Wall -g

# archivos base
SRC = cut.c srep.c count.c archivos.c

# objetos
OBJ = cut.o srep.o count.o archivos.o

# archivos finales ejecutables por consola
EXECUTABLES = cut srep count

# target por defecto
all: $(EXECUTABLES)

# reglas del makefile
cut: cut.o archivos.o
	$(CC) -o $@ $^

srep: srep.o archivos.o
	$(CC) -o $@ $^

count: count.o archivos.o
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
