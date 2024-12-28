#include "profesor.h"
#include "estudiante.h"
#include "curso.h"
#include "materia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Profesor profesores[MAX_PROFESORES];
static int num_profesores = 0;

extern Curso cursos[MAX_CURSOS];
extern int num_cursos;

void cargarProfesoresDesdeArchivo(const char *archivo) {
    FILE *file = fopen(archivo, "r");
    if (!file) {
        printf("No se pudo abrir el archivo de profesores.\n");
        return;
    }
    num_profesores = 0;
    char linea[512];
    while (fgets(linea, sizeof(linea), file)) {
        linea[strcspn(linea, "\n")] = '\0';
        char *t1 = strtok(linea, "-");
        char *t2 = strtok(NULL, "-");
        char *t3 = strtok(NULL, "-");
        char *t4 = strtok(NULL, "-");
        char *t5 = strtok(NULL, "-");
        char *t6 = strtok(NULL, "-");
        if (!t1 || !t2 || !t3 || !t4 || !t5 || !t6) continue;
        strcpy(profesores[num_profesores].nombres,   t1);
        strcpy(profesores[num_profesores].apellidos, t2);
        strcpy(profesores[num_profesores].cc,        t3);
        strcpy(profesores[num_profesores].usuario,   t4);
        strcpy(profesores[num_profesores].clave,     t5);
        strcpy(profesores[num_profesores].estado,    t6);
        profesores[num_profesores].num_materias = 0;
        char *resto = strtok(NULL, "");
        if (resto && strlen(resto) > 0) {
            if (resto[0] == '-') memmove(resto, resto + 1, strlen(resto));
            char *m = strtok(resto, "/");
            while (m && profesores[num_profesores].num_materias < MAX_MATERIAS) {
                if (strlen(m) > 0) {
                    strcpy(profesores[num_profesores].materias[profesores[num_profesores].num_materias], m);
                    profesores[num_profesores].num_materias++;
                }
                m = strtok(NULL, "/");
            }
        }
        num_profesores++;
        if (num_profesores == MAX_PROFESORES) {
            fclose(file);
            return;
        }
    }
    fclose(file);
}

void guardarProfesoresEnArchivo(const char *archivo) {
    FILE *file = fopen(archivo, "w");
    if (!file) {
        printf("No se pudo guardar en el archivo de profesores.\n");
        return;
    }
    for (int i = 0; i < num_profesores; i++) {
        fprintf(file, "%s-%s-%s-%s-%s-%s",
                profesores[i].nombres,
                profesores[i].apellidos,
                profesores[i].cc,
                profesores[i].usuario,
                profesores[i].clave,
                profesores[i].estado);
        if (profesores[i].num_materias > 0) {
            fprintf(file, "-");
            for (int j = 0; j < profesores[i].num_materias; j++) {
                fprintf(file, "%s", profesores[i].materias[j]);
                if (j < profesores[i].num_materias - 1) fprintf(file, "/");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int buscarProfesorPorCC(const char *cc) {
    for (int i = 0; i < num_profesores; i++) {
        if (strcmp(profesores[i].cc, cc) == 0) {
            return i;
        }
    }
    return -1;
}

int buscarProfesorPorUsuario(const char *usuario) {
    for (int i = 0; i < num_profesores; i++) {
        if (strcmp(profesores[i].usuario, usuario) == 0) {
            return i;
        }
    }
    return -1;
}

int tieneCursosActivos(const char *cc) {
    for (int i = 0; i < num_cursos; i++) {
        // Verifica si el profesor tiene asignado el curso
        if (strcmp(cursos[i].profesor_cc, cc) == 0) {
            if (cursoEstaEnProceso(&cursos[i])) {
                return 1;
            } else {
                printf("DEBUG: Curso no encontrado");
            }
        }
    }
    return 0;
}

void crearProfesor() {
    if (num_profesores >= MAX_PROFESORES) {
        printf("No se pueden anadir mas profesores.\n");
        return;
    }

    Profesor nuevo;

    printf("Ingrese los nombres: ");
    fgets(nuevo.nombres, sizeof(nuevo.nombres), stdin);
    nuevo.nombres[strcspn(nuevo.nombres, "\n")] = '\0';

    printf("Ingrese los apellidos: ");
    fgets(nuevo.apellidos, sizeof(nuevo.apellidos), stdin);
    nuevo.apellidos[strcspn(nuevo.apellidos, "\n")] = '\0';

    {
        int cc_valida = 0;
        while (!cc_valida) {
            printf("Ingrese la C.C.: ");
            fgets(nuevo.cc, sizeof(nuevo.cc), stdin);
            nuevo.cc[strcspn(nuevo.cc, "\n")] = '\0';

            if (buscarProfesorPorCC(nuevo.cc) != -1) {
                printf("Ya existe un profesor con esa C.C. Intente nuevamente.\n");
            } else if (buscarEstudiantePorMat(nuevo.cc) != -1) {
                printf("La C.C. coincide con la matricula de un estudiante. Intente nuevamente.\n");
            } else {
                cc_valida = 1;
            }
        }
    }

    {
        int usuario_valido = 0;
        while (!usuario_valido) {
            printf("Ingrese el usuario: ");
            fgets(nuevo.usuario, sizeof(nuevo.usuario), stdin);
            nuevo.usuario[strcspn(nuevo.usuario, "\n")] = '\0';

            if (buscarProfesorPorUsuario(nuevo.usuario) != -1) {
                printf("Ya existe un profesor con ese usuario. Intente nuevamente.\n");
            } else if (buscarEstudiantePorUsuario(nuevo.usuario) != -1) {
                printf("El usuario coincide con el de un estudiante. Intente nuevamente.\n");
            } else {
                usuario_valido = 1;
            }
        }
    }

    printf("Ingrese la clave: ");
    fgets(nuevo.clave, sizeof(nuevo.clave), stdin);
    nuevo.clave[strcspn(nuevo.clave, "\n")] = '\0';

    strcpy(nuevo.estado, "Activo");

    printf("Ingrese el numero de materias: ");
    scanf("%d", &nuevo.num_materias);
    getchar();

    for (int i = 0; i < nuevo.num_materias; i++) {
        int materia_valida = 0;
        while (!materia_valida) {
            printf("Ingrese el codigo de la materia %d: ", i + 1);
            fgets(nuevo.materias[i], sizeof(nuevo.materias[i]), stdin);
            nuevo.materias[i][strcspn(nuevo.materias[i], "\n")] = '\0';

            if (buscarMateriaPorCodigo(nuevo.materias[i]) == -1) {
                printf("La materia %s no existe. Intente nuevamente.\n", nuevo.materias[i]);
            } else {
                materia_valida = 1;
            }
        }
    }

    profesores[num_profesores++] = nuevo;
    printf("Profesor creado con exito.\n");
}

void editarProfesor() {
    char cc[20];
    printf("Ingrese la C.C. del profesor a editar: ");
    fgets(cc, sizeof(cc), stdin);
    cc[strcspn(cc, "\n")] = '\0';

    int indice = buscarProfesorPorCC(cc);
    if (indice == -1) {
        printf("Profesor no encontrado.\n");
        return;
    }

    Profesor *prof = &profesores[indice];

    printf("Ingrese la nueva clave (max 20 caracteres): ");
    fgets(prof->clave, sizeof(prof->clave), stdin);
    prof->clave[strcspn(prof->clave, "\n")] = '\0';

    char nuevo_estado[10];
    printf("Ingrese el nuevo estado (Activo/Inactivo): ");
    fgets(nuevo_estado, sizeof(nuevo_estado), stdin);
    nuevo_estado[strcspn(nuevo_estado, "\n")] = '\0';

    if (strcmp(nuevo_estado, "Inactivo") == 0) {
        if (tieneCursosActivos(prof->cc)) {
            printf("No se puede inactivar un profesor con cursos en proceso.\n");
            return;
        }
    }

    strcpy(prof->estado, nuevo_estado);
    printf("Profesor editado con exito.\n");
}
