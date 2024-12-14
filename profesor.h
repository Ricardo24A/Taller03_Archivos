#ifndef PROFESORES_H
#define PROFESORES_H

#include <stdlib.h>

#define MAX_MATERIAS 10

typedef struct {
    char nombres[50];
    char apellidos[50];
    char cc[20];
    char usuario[30];
    char clave[20];
    char estado[10];
    char materias[MAX_MATERIAS][30];
    int num_materias;
} Profesor;

void cargarProfesoresDesdeArchivo(const char *archivo);
void guardarProfesoresEnArchivo(const char *archivo);
void crearProfesor();
void editarProfesor();
int buscarProfesorPorCC(const char *cc);
int buscarProfesorPorUsuario(const char *usuario);

#endif
