CC = gcc
CFLAGS = -c -I.

programa: main.o curso.o estudiante.o materia.o menu.o profesor.o
	$(CC) -o programa main.o curso.o estudiante.o materia.o menu.o profesor.o

main.o: main.c
	$(CC) $(CFLAGS) main.c

curso.o: curso.c
	$(CC) $(CFLAGS) curso.c

estudiante.o: estudiante.c
	$(CC) $(CFLAGS) estudiante.c

materia.o: materia.c
	$(CC) $(CFLAGS) materia.c

menu.o: menu.c
	$(CC) $(CFLAGS) menu.c

profesor.o: profesor.c
	$(CC) $(CFLAGS) profesor.c

.PHONY: clean
clean:
	rm -f *.o programa
