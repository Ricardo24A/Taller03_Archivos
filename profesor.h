#ifndef PROFESORES_H
#define PROFESORES_H

#include <stdlib.h>

#ifndef MAX_MATERIAS
#define MAX_MATERIAS 10
#endif

#ifndef MAX_PROFESORES
#define MAX_PROFESORES 100 // Asegúrate de definir un valor adecuado para el número de profesores
#endif

#ifndef MAX_ESTUDIANTES
#define MAX_ESTUDIANTES 100 // Asegúrate de definir un valor adecuado para el número de estudiantes
#endif

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

// Funciones relacionadas con los profesores
void cargarProfesoresDesdeArchivo(const char *archivo);  // Cargar los profesores desde un archivo
void guardarProfesoresEnArchivo(const char *archivo);   // Guardar los profesores en un archivo
void crearProfesor();  // Crear un nuevo profesor
void editarProfesor();  // Editar un profesor existente
int buscarProfesorPorCC(const char *cc);  // Buscar un profesor por su C.C.
int buscarProfesorPorUsuario(const char *usuario);  // Buscar un profesor por su usuario
int tieneCursosActivos(const char *cc);
#endif
