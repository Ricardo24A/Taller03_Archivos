#ifndef ESTUDIANTES_H
#define ESTUDIANTES_H

#define MAX_ESTUDIANTES 100

typedef struct {
    char nombres[50];
    char apellidos[50];
    char matricula[15];
    char usuario[30];
    char clave[30];
    char estado[10];
} Estudiante;

// Funciones para manejar estudiantes
void cargarEstudiantesDesdeArchivo(const char *archivo);
void guardarEstudiantesEnArchivo(const char *archivo);
void crearEstudiante();
void editarEstudiante();

#endif // ESTUDIANTES_H
