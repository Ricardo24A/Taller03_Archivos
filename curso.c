#include "curso.h"
#include "profesor.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static Curso *cursos = NULL;
static int num_cursos = 0;
static int capacidad_cursos = 10;


void cargarCursosDelArchivo(const char *archivo) {
    inicializarCursos();

    FILE *file = fopen(archivo, "r");
    if (!file) {
        printf("No se pudo abrir el archivo de cursos.\n");
        return;
    }

    char AlmacenarChars[256];
    while (fgets(AlmacenarChars, sizeof(AlmacenarChars), file)) {
        if (num_cursos >= capacidad_cursos) {
            expandirCursos();
        }

        Curso nuevoCurso;
        char estudiantes_linea[256];

        sscanf(AlmacenarChars, "%9[^-]-%29[^-]-%19[^-]-%10[^-]-%10[^-]",
               nuevoCurso.codigo,
               nuevoCurso.materia,
               nuevoCurso.profesor_cc,
               nuevoCurso.fecha_inicio,
               nuevoCurso.fecha_fin);

        if (fgets(estudiantes_linea, sizeof(estudiantes_linea), file)) {
            nuevoCurso.estudiantes = malloc(MAX_ESTUDIANTES * sizeof(char *));
            nuevoCurso.num_estudiantes = 0;

            char *separador = strtok(estudiantes_linea, "/");
            while (separador && nuevoCurso.num_estudiantes < MAX_ESTUDIANTES) {
                nuevoCurso.estudiantes[nuevoCurso.num_estudiantes] = malloc(30 * sizeof(char));
                strncpy(nuevoCurso.estudiantes[nuevoCurso.num_estudiantes++], separador, 29);
                separador = strtok(NULL, "/");
            }
        }

        cursos[num_cursos++] = nuevoCurso;
    }

    fclose(file);
}

void guardarCursosArchivo(const char *archivo) {
    FILE *file = fopen(archivo, "w");
    if (!file) {
        printf("No se pudo guardar en el archivo de cursos.\n");
        return;
    }

    for (int i = 0; i < num_cursos; i++) {
        fprintf(file, "%s-%s-%s-%s-%s\n",
                cursos[i].codigo,
                cursos[i].materia,
                cursos[i].profesor_cc,
                cursos[i].fecha_inicio,
                cursos[i].fecha_fin);

        for (int j = 0; j < cursos[i].num_estudiantes; j++) {
            fprintf(file, "%s%s", cursos[i].estudiantes[j], (j < cursos[i].num_estudiantes - 1) ? "/" : "\n");
        }
    }

    fclose(file);
}


void crearCurso() {
    inicializarCursos();

    if (num_cursos >= capacidad_cursos) {
        expandirCursos();
    }

    Curso nuevoCurso;
    printf("Ingrese el codigo del curso: ");
    fgets(nuevoCurso.codigo, sizeof(nuevoCurso.codigo), stdin);
    nuevoCurso.codigo[strcspn(nuevoCurso.codigo, "\n")] = '\0';

    if (buscarCursoPorCodigo(nuevoCurso.codigo) != -1) {
        printf("El codigo ya existe.\n");
        return;
    }

    printf("Ingrese la materia: ");
    fgets(nuevoCurso.materia, sizeof(nuevoCurso.materia), stdin);
    nuevoCurso.materia[strcspn(nuevoCurso.materia, "\n")] = '\0';

    char cc[20];
    do {
        printf("Ingrese la C.C. del profesor: ");
        fgets(cc, sizeof(cc), stdin);
        cc[strcspn(cc, "\n")] = '\0';
    } while (buscarProfesorPorCC(cc) == -1);

    strcpy(nuevoCurso.profesor_cc, cc);

    printf("Ingrese la fecha de inicio (YYYY-MM-DD): ");
    fgets(nuevoCurso.fecha_inicio, sizeof(nuevoCurso.fecha_inicio), stdin);
    nuevoCurso.fecha_inicio[strcspn(nuevoCurso.fecha_inicio, "\n")] = '\0';

    printf("Ingrese la fecha de fin (YYYY-MM-DD): ");
    fgets(nuevoCurso.fecha_fin, sizeof(nuevoCurso.fecha_fin), stdin);
    nuevoCurso.fecha_fin[strcspn(nuevoCurso.fecha_fin, "\n")] = '\0';

    if (compararFechas(nuevoCurso.fecha_inicio, nuevoCurso.fecha_fin) > 0) {
        printf("La fecha de inicio no puede ser mayor que la fecha de fin.\n");
        return;
    }

    nuevoCurso.estudiantes = malloc(MAX_ESTUDIANTES * sizeof(char *));
    nuevoCurso.num_estudiantes = 0;

    cursos[num_cursos++] = nuevoCurso;
    guardarCursosEnArchivo("cursos.txt");
    printf("Curso creado con exito.\n");
}


void editarCurso() {
    cargarCursosDesdeArchivo("cursos.txt");
    char codigo[10];
    printf("Ingrese el codigo del curso: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    int indice = buscarCursoPorCodigo(codigo);
    if (indice == -1) {
        printf("Curso no encontrado.\n");
        return;
    }

    Curso *curso = &cursos[indice];

    char nueva_fecha_inicio[11];
    printf("Ingrese la nueva fecha de inicio (YYYY-MM-DD): ");
    fgets(nueva_fecha_inicio, sizeof(nueva_fecha_inicio), stdin);
    nueva_fecha_inicio[strcspn(nueva_fecha_inicio, "\n")] = '\0';

    if (compararFechas(nueva_fecha_inicio, curso->fecha_inicio) <= 0) {
        strcpy(curso->fecha_inicio, nueva_fecha_inicio);
        guardarCursosEnArchivo("cursos.txt");
        printf("Curso editado con exito.\n");
    } else {
        printf("No se puede editar un curso cuya fecha de inicio ya comenzo.\n");
    }
}

void inicializarLosCursos() {
    if (cursos == NULL) {
        cursos = malloc(capacidad_cursos * sizeof(Curso));
    }
}

void expandirCursos() {
    Curso *nuevo_bloque = malloc((capacidad_cursos + 10) * sizeof(Curso));
    if (nuevo_bloque) {
        memcpy(nuevo_bloque, cursos, capacidad_cursos * sizeof(Curso));
        free(cursos);
        cursos = nuevo_bloque;
        capacidad_cursos += 10;
    } else {
        printf("Error al asignar memoria adicional para cursos.\n");
    }
}


int buscarCursoPorCodigo(const char *codigo) {
    for (int i = 0; i < num_cursos; i++) {
        if (strcmp(cursos[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}


int compararFechas(const char *fecha1, const char *fecha2) {
    struct tm tm1, tm2;
    strptime(fecha1, "%Y-%m-%d", &tm1);
    strptime(fecha2, "%Y-%m-%d", &tm2);

    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);

    return difftime(time1, time2);
}
