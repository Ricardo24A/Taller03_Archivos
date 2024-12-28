#ifndef ESTUDIANTES_H
#define ESTUDIANTES_H

#ifndef MAX_ESTUDIANTES
#define MAX_ESTUDIANTES 100
#endif

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
int buscarEstudiantePorMat(const char *matricula);
int buscarEstudiantePorUsuario(const char *usuario);
int estudianteTieneCursos(const char *mat);
#endif // ESTUDIANTES_H
