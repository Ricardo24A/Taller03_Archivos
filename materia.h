#ifndef MATERIAS_H
#define MATERIAS_H

#ifndef MAX_MATERIAS
#define MAX_MATERIAS 100
#endif

typedef struct {
    char nombre[50];
    char codigo[10];
    char estado[10]; // Activo/Inactivo
} Materia;

// Funciones para manejar materias
void cargarMateriasDesdeArchivo(const char *archivo);
void guardarMateriasEnArchivo(const char *archivo);
void crearMateria();
void editarMateria();

#endif // MATERIAS_H
