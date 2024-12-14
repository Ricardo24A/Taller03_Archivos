#include "profesores.h"
#include <stdio.h>
#include <string.h>

static Profesor *profesores = NULL;
static int num_profesores = 0;
static int capacidad_profesores = 10;


void inicializarProfesores() {
    if (profesores == NULL) {
        profesores = malloc(capacidad_profesores * sizeof(Profesor));
    }
}

void expandirProfesores() {
    Profesor *nuevo_bloque = malloc((capacidad_profesores + 10) * sizeof(Profesor));
    if (nuevo_bloque) {
        memcpy(nuevo_bloque, profesores, capacidad_profesores * sizeof(Profesor));
        free(profesores);
        profesores = nuevo_bloque;
        capacidad_profesores += 10;
    } else {
        printf("Error al asignar memoria adicional para profesores.\n");
    }
}


void cargarProfesoresDesdeArchivo(const char *archivo) {
    inicializarProfesores();

    FILE *file = fopen(archivo, "r");
    if (!file) {
        printf("No se pudo abrir el archivo de profesores.\n");
        return;
    }

    char AlmacenamientoChars[256];
    while (fgets(AlmacenamientoChars, sizeof(AlmacenamientoChars), file)) {
        if (num_profesores >= capacidad_profesores) {
            expandirProfesores();
        }

        Profesor nuevoProf;
        char materias[256];

        sscanf(AlmacenamientoChars, "%49[^-]-%49[^-]-%19[^-]-%29[^-]-%19[^-]-%9[^-]",
               nuevoProf.nombres,
               nuevoProf.apellidos,
               nuevoProf.cc,
               nuevoProf.usuario,
               nuevoProf.clave,
               nuevoProf.estado);

        if (fgets(materias, sizeof(materias), file)) {
            char *separador = strtok(materias, "/");
            nuevoProf.num_materias = 0;
            while (separador && nuevoProf.num_materias < MAX_MATERIAS) {
                strncpy(nuevoProf.materias[nuevoProf.num_materias++], separador, sizeof(nuevoProf.materias[0]) - 1);
                separador = strtok(NULL, "/");
            }
        }

        profesores[num_profesores++] = nuevoProf;
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
        fprintf(file, "%s-%s-%s-%s-%s-%s\n",
                profesores[i].nombres,
                profesores[i].apellidos,
                profesores[i].cc,
                profesores[i].usuario,
                profesores[i].clave,
                profesores[i].estado);

        for (int j = 0; j < profesores[i].num_materias; j++) {
            fprintf(file, "%s%s", profesores[i].materias[j], (j < profesores[i].num_materias - 1) ? "/" : "\n");
        }
    }

    fclose(file);
}


void crearProfesor() {
    inicializarProfesores();

    if (num_profesores >= capacidad_profesores) {
        expandirProfesores();
    }

    Profesor nuevo;
    printf("Ingrese los nombres: ");
    fgets(nuevo.nombres, sizeof(nuevo.nombres), stdin);
    nuevo.nombres[strcspn(nuevo.nombres, "\n")] = '\0';

    printf("Ingrese los apellidos: ");
    fgets(nuevo.apellidos, sizeof(nuevo.apellidos), stdin);
    nuevo.apellidos[strcspn(nuevo.apellidos, "\n")] = '\0';

    do {
        printf("Ingrese la C.C.: ");
        fgets(nuevo.cc, sizeof(nuevo.cc), stdin);
        nuevo.cc[strcspn(nuevo.cc, "\n")] = '\0';
    } while (buscarProfesorPorCC(nuevo.cc) != -1);

    do {
        printf("Ingrese el usuario: ");
        fgets(nuevo.usuario, sizeof(nuevo.usuario), stdin);
        nuevo.usuario[strcspn(nuevo.usuario, "\n")] = '\0';
    } while (buscarProfesorPorUsuario(nuevo.usuario) != -1);

    printf("Ingrese la clave: ");
    fgets(nuevo.clave, sizeof(nuevo.clave), stdin);
    nuevo.clave[strcspn(nuevo.clave, "\n")] = '\0';

    strcpy(nuevo.estado, "Activo");

    printf("Ingrese el número de materias: ");
    scanf(" %d", &nuevo.num_materias);
    getchar();

    for (int i = 0; i < nuevo.num_materias; i++) {
        printf("Ingrese el nombre de la materia %d: ", i + 1);
        fgets(nuevo.materias[i], sizeof(nuevo.materias[i]), stdin);
        nuevo.materias[i][strcspn(nuevo.materias[i], "\n")] = '\0';
    }

    profesores[num_profesores++] = nuevo;
    guardarProfesoresEnArchivo("profesores.txt");
    printf("Profesor creado con éxito.\n");
}


void editarProfesor() {
    cargarProfesoresDesdeArchivo("profesores.txt");
    char cc[20];
    printf("Ingrese la C.C. del profesor: ");
    fgets(cc, sizeof(cc), stdin);
    cc[strcspn(cc, "\n")] = '\0';

    int indice = buscarProfesorPorCC(cc);
    if (indice == -1) {
        printf("Profesor no encontrado.\n");
        return;
    }

    Profesor *profesor = &profesores[indice];

    printf("Ingrese la nueva clave: ");
    fgets(profesor->clave, sizeof(profesor->clave), stdin);
    profesor->clave[strcspn(profesor->clave, "\n")] = '\0';

    char nuevo_estado[10];
    printf("Ingrese el nuevo estado (Activo/Inactivo): ");
    fgets(nuevo_estado, sizeof(nuevo_estado), stdin);
    nuevo_estado[strcspn(nuevo_estado, "\n")] = '\0';

    if (strcmp(nuevo_estado, "Inactivo") == 0) {
        printf("No se puede inactivar un profesor con cursos en proceso.\n");
        return;
    }

    strcpy(profesor->estado, nuevo_estado);
    guardarProfesoresEnArchivo("profesores.txt");
    printf("Profesor editado con éxito.\n");
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
