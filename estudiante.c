#include "estudiante.h"
#include "profesor.h"
#include "curso.h"
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
        printf("No se pueden anadir mas estudiantes.\n");
        return;
    }

    Estudiante nuevoEstudiante;

    printf("Ingrese los nombres del estudiante: ");
    fgets(nuevoEstudiante.nombres, sizeof(nuevoEstudiante.nombres), stdin);
    nuevoEstudiante.nombres[strcspn(nuevoEstudiante.nombres, "\n")] = '\0';

    printf("Ingrese los apellidos del estudiante: ");
    fgets(nuevoEstudiante.apellidos, sizeof(nuevoEstudiante.apellidos), stdin);
    nuevoEstudiante.apellidos[strcspn(nuevoEstudiante.apellidos, "\n")] = '\0';

    {
        int matricula_valida = 0;
        while (!matricula_valida) {
            printf("Ingrese la matricula: ");
            fgets(nuevoEstudiante.matricula, sizeof(nuevoEstudiante.matricula), stdin);
            nuevoEstudiante.matricula[strcspn(nuevoEstudiante.matricula, "\n")] = '\0';

            /* Verificar si la matricula ya existe en estudiantes */
            int repetida_estudiante = 0;
            for (int i = 0; i < num_estudiantes; i++) {
                if (strcmp(estudiantes[i].matricula, nuevoEstudiante.matricula) == 0) {
                    repetida_estudiante = 1;
                }
            }

            /* Verificar si coincide con la C.C. de algun profesor */
            int coincide_profesor = (buscarProfesorPorCC(nuevoEstudiante.matricula) != -1);

            if (repetida_estudiante) {
                printf("La matricula ya existe. Intente nuevamente.\n");
            } else if (coincide_profesor) {
                printf("La matricula coincide con la C.C. de un profesor. Intente nuevamente.\n");
            } else {
                matricula_valida = 1;
            }
        }
    }

    {
        int usuario_valido = 0;
        while (!usuario_valido) {
            printf("Ingrese el usuario: ");
            fgets(nuevoEstudiante.usuario, sizeof(nuevoEstudiante.usuario), stdin);
            nuevoEstudiante.usuario[strcspn(nuevoEstudiante.usuario, "\n")] = '\0';

            int repetido_estudiante = 0;
            for (int i = 0; i < num_estudiantes; i++) {
                if (strcmp(estudiantes[i].usuario, nuevoEstudiante.usuario) == 0) {
                    repetido_estudiante = 1;
                }
            }

            int coincide_profesor = (buscarProfesorPorUsuario(nuevoEstudiante.usuario) != -1);

            if (repetido_estudiante) {
                printf("El usuario ya existe en otro estudiante. Intente nuevamente.\n");
            } else if (coincide_profesor) {
                printf("El usuario coincide con el de un profesor. Intente nuevamente.\n");
            } else {
                usuario_valido = 1;
            }
        }
    }

    printf("Ingrese la clave: ");
    fgets(nuevoEstudiante.clave, sizeof(nuevoEstudiante.clave), stdin);
    nuevoEstudiante.clave[strcspn(nuevoEstudiante.clave, "\n")] = '\0';

    strcpy(nuevoEstudiante.estado, "Activo");

    estudiantes[num_estudiantes] = nuevoEstudiante;
    num_estudiantes++;

    printf("Estudiante creado con exito.\n");
}

// Editar un estudiante
void editarEstudiante() {
    char matricula[15];
    printf("Ingrese la matrícula del estudiante: ");
    fgets(matricula, sizeof(matricula), stdin);
    matricula[strcspn(matricula, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < num_estudiantes; i++) {
        if (strcmp(estudiantes[i].matricula, matricula) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Estudiante no encontrado.\n");
        return;
    }

    Estudiante *estudiante = &estudiantes[encontrado];

    printf("Ingrese la nueva clave: ");
    fgets(estudiante->clave, sizeof(estudiante->clave), stdin);
    estudiante->clave[strcspn(estudiante->clave, "\n")] = '\0';

    char nuevo_estado[10];
    printf("Ingrese el nuevo estado (Activo/Inactivo): ");
    fgets(nuevo_estado, sizeof(nuevo_estado), stdin);
    nuevo_estado[strcspn(nuevo_estado, "\n")] = '\0';

    // Verificar si el estudiante tiene cursos antes de inactivarlo
    if (strcmp(nuevo_estado, "Inactivo") == 0) {
        if (estudianteTieneCursos(estudiante->matricula)) {
            printf("No se puede inactivar un estudiante que está en un curso.\n");
            return;
        }
    }

    strcpy(estudiante->estado, nuevo_estado);
    printf("Estudiante editado con éxito.\n");
}

int buscarEstudiantePorMat(const char *matricula) {
    // Aquí verificamos que la matrícula del estudiante coincida con la ingresada.
    for (int i = 0; i < num_estudiantes; i++) {
        if (strcmp(estudiantes[i].matricula, matricula) == 0) {
            return i;
        }
    }
    return -1;
}

// Buscar un estudiante por usuario
int buscarEstudiantePorUsuario(const char *usuario) {
    for (int i = 0; i < num_estudiantes; i++) {
        if (strcmp(estudiantes[i].usuario, usuario) == 0) {
            return i;
        }
    }
    return -1;
}

int estudianteTieneCursos(const char *mat) {
    for (int i = 0; i < num_cursos; i++) {
        if (cursoEstaEnProceso(&cursos[i])) {
            for (int j = 0; j < cursos[i].num_estudiantes; j++) {
                if (strcmp(cursos[i].estudiantes[j], mat) == 0) {
                    return 1; // Estudiante encontrado en un curso en proceso
                }
            }
        }
    }
    return 0;
}
