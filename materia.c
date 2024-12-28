#include "materia.h"
#include "curso.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Materia materias[MAX_MATERIAS];
static int num_materias = 0;

void cargarMateriasDesdeArchivo(const char *archivo) {
    FILE *file = fopen(archivo, "r");
    if (!file) {
        printf("No se pudo abrir el archivo de materias.\n");
        return;
    }

    while (fscanf(file, "%49[^-]-%9[^-]-%9s\n",
                  materias[num_materias].nombre,
                  materias[num_materias].codigo,
                  materias[num_materias].estado) == 3) {
        num_materias++;
    }

    fclose(file);
}

void guardarMateriasEnArchivo(const char *archivo) {
    FILE *file = fopen(archivo, "w");
    if (!file) {
        printf("No se pudo guardar en el archivo de materias.\n");
        return;
    }

    for (int i = 0; i < num_materias; i++) {
        fprintf(file, "%s-%s-%s\n",
                materias[i].nombre,
                materias[i].codigo,
                materias[i].estado);
    }

    fclose(file);
}

void crearMateria() {
    if (num_materias >= MAX_MATERIAS) {
        printf("No se pueden agregar mas materias.\n");
        return;
    }

    Materia nuevaMateria;
    printf("Ingrese el nombre de la materia: ");
    fgets(nuevaMateria.nombre, sizeof(nuevaMateria.nombre), stdin);
    nuevaMateria.nombre[strcspn(nuevaMateria.nombre, "\n")] = '\0';

    printf("Ingrese el codigo de la materia: ");
    fgets(nuevaMateria.codigo, sizeof(nuevaMateria.codigo), stdin);
    nuevaMateria.codigo[strcspn(nuevaMateria.codigo, "\n")] = '\0';

    // Verificar que el código no exista ya
    for (int i = 0; i < num_materias; i++) {
        if (strcmp(materias[i].codigo, nuevaMateria.codigo) == 0) {
            printf("El codigo ya existe.\n");
            return;
        }
    }

    strcpy(nuevaMateria.estado, "Activo");

    materias[num_materias++] = nuevaMateria;
    printf("Materia creada con exito.\n");
}

void editarMateria() {
    char codigo[10];
    printf("Ingrese el codigo de la materia: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    for (int i = 0; i < num_materias; i++) {
        if (strcmp(materias[i].codigo, codigo) == 0) {
            printf("Ingrese el nuevo estado (Activo/Inactivo): ");
            char nuevoEstado[10];
            fgets(nuevoEstado, sizeof(nuevoEstado), stdin);
            nuevoEstado[strcspn(nuevoEstado, "\n")] = '\0';

            if (strcmp(nuevoEstado, "Inactivo") == 0) {
                // Validacion: verificar si la materia esta asociada a cursos en proceso
                if (materiaEnCurso(codigo)) {
                    printf("No se puede inactivar una materia con cursos en proceso.\n");
                    return;
                }
            }

            strcpy(materias[i].estado, nuevoEstado);
            printf("Materia editada con exito.\n");
            return;
        }
    }

    printf("La materia con el codigo %s no existe.\n", codigo);
}

int buscarMateriaPorCodigo(const char *codigo) {
    for (int i = 0; i < num_materias; i++) {
        if (strcmp(materias[i].codigo, codigo) == 0) {
            return i; // Se encuentra la materia
        }
    }
    return -1;
}

int materiaEnCurso(const char *codigoMateria) {
    for (int i = 0; i < num_cursos; i++) {
        if (strcmp(cursos[i].materia, codigoMateria) == 0 && cursoEstaEnProceso(&cursos[i])) {
            return 1;
        }
    }
    return 0;
}
