#include "estudiante.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Estudiante estudiantes[MAX_ESTUDIANTES];
static int num_estudiantes = 0;

void cargarEstudiantesDesdeArchivo(const char *archivo) {
    FILE *file = fopen(archivo, "r");
    if (!file) {
        printf("No se pudo abrir el archivo de estudiantes.\n");
        return;
    }

    while (fscanf(file, "%49[^-]-%49[^-]-%14[^-]-%29[^-]-%29[^-]-%9s\n",
                  estudiantes[num_estudiantes].nombres,
                  estudiantes[num_estudiantes].apellidos,
                  estudiantes[num_estudiantes].matricula,
                  estudiantes[num_estudiantes].usuario,
                  estudiantes[num_estudiantes].clave,
                  estudiantes[num_estudiantes].estado) == 6) {
        num_estudiantes++;
    }

    fclose(file);
}

void guardarEstudiantesEnArchivo(const char *archivo) {
    FILE *file = fopen(archivo, "w");
    if (!file) {
        printf("No se pudo guardar en el archivo de estudiantes.\n");
        return;
    }

    for (int i = 0; i < num_estudiantes; i++) {
        fprintf(file, "%s-%s-%s-%s-%s-%s\n",
                estudiantes[i].nombres,
                estudiantes[i].apellidos,
                estudiantes[i].matricula,
                estudiantes[i].usuario,
                estudiantes[i].clave,
                estudiantes[i].estado);
    }

    fclose(file);
}

void crearEstudiante() {
    if (num_estudiantes >= MAX_ESTUDIANTES) {
        printf("No se pueden agregar más estudiantes.\n");
        return;
    }

    Estudiante nuevoEstudiante;
    printf("Ingrese los nombres del estudiante: ");
    fgets(nuevoEstudiante.nombres, sizeof(nuevoEstudiante.nombres), stdin);
    nuevoEstudiante.nombres[strcspn(nuevoEstudiante.nombres, "\n")] = '\0';

    printf("Ingrese los apellidos del estudiante: ");
    fgets(nuevoEstudiante.apellidos, sizeof(nuevoEstudiante.apellidos), stdin);
    nuevoEstudiante.apellidos[strcspn(nuevoEstudiante.apellidos, "\n")] = '\0';

    printf("Ingrese la matrícula: ");
    fgets(nuevoEstudiante.matricula, sizeof(nuevoEstudiante.matricula), stdin);
    nuevoEstudiante.matricula[strcspn(nuevoEstudiante.matricula, "\n")] = '\0';

    for (int i = 0; i < num_estudiantes; i++) {
        if (strcmp(estudiantes[i].matricula, nuevoEstudiante.matricula) == 0) {
            printf("La matrícula ya existe.\n");
            return;
        }
    }

    printf("Ingrese el usuario: ");
    fgets(nuevoEstudiante.usuario, sizeof(nuevoEstudiante.usuario), stdin);
    nuevoEstudiante.usuario[strcspn(nuevoEstudiante.usuario, "\n")] = '\0';

    printf("Ingrese la clave: ");
    fgets(nuevoEstudiante.clave, sizeof(nuevoEstudiante.clave), stdin);
    nuevoEstudiante.clave[strcspn(nuevoEstudiante.clave, "\n")] = '\0';

    strcpy(nuevoEstudiante.estado, "Activo");

    estudiantes[num_estudiantes++] = nuevoEstudiante;
    guardarEstudiantesEnArchivo("estudiantes.txt");
    printf("Estudiante creado con éxito.\n");
}

void editarEstudiante() {
    char matricula[15];
    printf("Ingrese la matrícula del estudiante: ");
    fgets(matricula, sizeof(matricula), stdin);
    matricula[strcspn(matricula, "\n")] = '\0';

    for (int i = 0; i < num_estudiantes; i++) {
        if (strcmp(estudiantes[i].matricula, matricula) == 0) {
            printf("Ingrese la nueva clave: ");
            fgets(estudiantes[i].clave, sizeof(estudiantes[i].clave), stdin);
            estudiantes[i].clave[strcspn(estudiantes[i].clave, "\n")] = '\0';

            printf("Ingrese el nuevo estado (Activo/Inactivo): ");
            fgets(estudiantes[i].estado, sizeof(estudiantes[i].estado), stdin);
            estudiantes[i].estado[strcspn(estudiantes[i].estado, "\n")] = '\0';

            guardarEstudiantesEnArchivo("estudiantes.txt");
            printf("Estudiante editado con éxito.\n");
            return;
        }
    }

    printf("Estudiante no encontrado.\n");
}
