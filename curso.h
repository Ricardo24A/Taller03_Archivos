#ifndef CURSOS_H
#define CURSOS_H

#include <stdlib.h>

#define MAX_ESTUDIANTES 30

typedef struct {
    char codigo[10];
    char materia[30];
    char profesor_cc[20];
    char fecha_inicio[11];
    char fecha_fin[11];
    char **estudiantes;
    int num_estudiantes;
} Curso;

void cargarCursosDesdeArchivo(const char *archivo);
void guardarCursosEnArchivo(const char *archivo);
void crearCurso();
void editarCurso();
int buscarCursoPorCodigo(const char *codigo);
int compararFechas(const char *fecha1, const char *fecha2);

#endif
