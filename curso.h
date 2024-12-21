
#ifndef CURSOS_H
#define CURSOS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifndef MAX_ESTUDIANTES
#define MAX_ESTUDIANTES 30
#endif

typedef struct {
    char codigo[10];
    char materia[30];
    char profesor_cc[20];
    char fecha_inicio[11];
    char fecha_fin[11];
    char **estudiantes;
    int num_estudiantes;
} Curso;

void cargarCursosDelArchivo(const char *archivo);
void guardarCursosEnArchivo(const char *archivo);
void crearCurso();
void editarCurso();
int buscarCursoPorCodigo(const char *codigo);
int compararFechas(const char *fecha1, const char *fecha2);
void inicializarLosCursos();
void expandirCursos();
#endif
