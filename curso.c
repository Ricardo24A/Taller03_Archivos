#include "curso.h"
#include "profesor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static Curso *cursos = NULL;
static int num_cursos = 0;
static int capacidad_cursos = 10;

void inicializarLosCursos() {
    if (cursos == NULL) {
        cursos = malloc(capacidad_cursos * sizeof(Curso));
        if (cursos == NULL) {
            printf("Error al asignar memoria para cursos.\n");
            exit(1);
        }
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
        exit(1);
    }
}

int compararFechas(const char *fecha1, const char *fecha2) {
    return strcmp(fecha1, fecha2);
}

int buscarCursoPorCodigo(const char *codigo) {
    for (int i = 0; i < num_cursos; i++) {
        if (strcmp(cursos[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}

void cargarCursosDelArchivo(const char *archivo) {
    inicializarLosCursos();

    FILE *file = fopen(archivo, "r");
    if (!file) {
        printf("No se pudo abrir el archivo de cursos.\n");
        return;
    }

    char AlmacenamientoChars[256];
    while (fgets(AlmacenamientoChars, sizeof(AlmacenamientoChars), file)) {
        if (num_cursos >= capacidad_cursos) {
            expandirCursos();
        }

        Curso nuevoCurso;
        char estudiantes_linea[256];

        sscanf(AlmacenamientoChars, "%9[^-]-%29[^-]-%19[^-]-%10[^-]-%10[^-]",
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

void guardarCursosEnArchivo(const char *archivo) {
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
    inicializarLosCursos();

    if (num_cursos >= capacidad_cursos) {
        expandirCursos();
    }

    Curso nuevoCurso;
    printf("\nIngrese el codigo del curso: ");
    fgets(nuevoCurso.codigo, sizeof(nuevoCurso.codigo), stdin);
    nuevoCurso.codigo[strcspn(nuevoCurso.codigo, "\n")] = '\0';

    if (buscarCursoPorCodigo(nuevoCurso.codigo) != -1) {
        printf("El codigo ya existe.\n");
        return;
    }

    printf("\nIngrese la materia: ");
    fgets(nuevoCurso.materia, sizeof(nuevoCurso.materia), stdin);
    nuevoCurso.materia[strcspn(nuevoCurso.materia, "\n")] = '\0';

    char cc[20];
    do {
        printf("\nIngrese la C.C. del profesor: ");
        fgets(cc, sizeof(cc), stdin);
        cc[strcspn(cc, "\n")] = '\0';
    } while (buscarProfesorPorCC(cc) == -1);

    strcpy(nuevoCurso.profesor_cc, cc);

    printf("\nIngrese la fecha de inicio (YYYY-MM-DD): ");
    fgets(nuevoCurso.fecha_inicio, sizeof(nuevoCurso.fecha_inicio), stdin);
    nuevoCurso.fecha_inicio[strcspn(nuevoCurso.fecha_inicio, "\n")] = '\0';

    // Limpiar el buffer de entrada después de fgets
    getchar();

    printf("\nIngrese la fecha de fin (YYYY-MM-DD): ");
    fgets(nuevoCurso.fecha_fin, sizeof(nuevoCurso.fecha_fin), stdin);
    nuevoCurso.fecha_fin[strcspn(nuevoCurso.fecha_fin, "\n")] = '\0';

    if (compararFechas(nuevoCurso.fecha_inicio, nuevoCurso.fecha_fin) > 0) {
        printf("La fecha de inicio no puede ser mayor que la fecha de fin.\n");
        return;
    }

    printf("\nIngrese el numero de estudiantes (maximo 30): ");
    int num_estudiantes;
    scanf("%d", &num_estudiantes);
    getchar();

    if (num_estudiantes > 30) {
        printf("No puede haber más de 30 estudiantes.\n");
        return;
    }

    nuevoCurso.estudiantes = malloc(num_estudiantes * sizeof(char *));
    nuevoCurso.num_estudiantes = num_estudiantes;

    for (int i = 0; i < num_estudiantes; i++) {
        nuevoCurso.estudiantes[i] = malloc(30 * sizeof(char));
        printf("Ingrese la matricula del estudiante %d: ", i + 1);
        fgets(nuevoCurso.estudiantes[i], 30, stdin);
        nuevoCurso.estudiantes[i][strcspn(nuevoCurso.estudiantes[i], "\n")] = '\0';
    }

    cursos[num_cursos++] = nuevoCurso;
}

void editarCurso() {
    char codigo[10];
    printf("\nIngrese el codigo del curso: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    int indice = buscarCursoPorCodigo(codigo);
    if (indice == -1) {
        printf("Curso no encontrado.\n");
        return;
    }

    Curso *curso = &cursos[indice];
    char nueva_fecha_inicio[11];
    printf("\nIngrese la nueva fecha de inicio (YYYY-MM-DD): ");
    fgets(nueva_fecha_inicio, sizeof(nueva_fecha_inicio), stdin);
    nueva_fecha_inicio[strcspn(nueva_fecha_inicio, "\n")] = '\0';

    if (compararFechas(nueva_fecha_inicio, curso->fecha_inicio) <= 0) {
        strcpy(curso->fecha_inicio, nueva_fecha_inicio);
        printf("Curso editado con exito.\n");
    } else {
        printf("No se puede editar un curso cuya fecha de inicio ya comenzo.\n");
    }

    getchar();
    char nueva_fecha_fin[11];
    printf("\nIngrese la nueva fecha de fin (YYYY-MM-DD): ");
    fgets(nueva_fecha_fin, sizeof(nueva_fecha_fin), stdin);
    nueva_fecha_fin[strcspn(nueva_fecha_fin, "\n")] = '\0';

    if (compararFechas(nueva_fecha_inicio, nueva_fecha_fin) > 0) {
        printf("La fecha de inicio no puede ser mayor que la fecha de fin.\n");
    } else {
        strcpy(curso->fecha_fin, nueva_fecha_fin);
        printf("Fecha de fin actualizada con exito.\n");
    }
}
