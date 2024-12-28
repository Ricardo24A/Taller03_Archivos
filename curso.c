#include "curso.h"
#include "profesor.h"
#include "estudiante.h"
#include "materia.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Curso cursos[MAX_CURSOS];
int num_cursos = 0;

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
    FILE *f = fopen(archivo, "r");
    if (!f) {
        printf("No se pudo abrir el archivo de cursos.\n");
        return;
    }

    num_cursos = 0;
    char linea[512];

    while (fgets(linea, sizeof(linea), f) && num_cursos < MAX_CURSOS) {
        linea[strcspn(linea, "\n")] = '\0';

        char *resto = NULL;

        char *t1 = strtok(linea, "-"); // Codigo
        char *t2 = strtok(NULL, "-"); // Materia
        char *t3 = strtok(NULL, "-"); // Profesor CC
        char *t4 = strtok(NULL, "-"); // Año Inicio
        char *t5 = strtok(NULL, "-"); // Mes Inicio
        char *t6 = strtok(NULL, "-"); // Día Inicio
        char *t7 = strtok(NULL, "-"); // Año Fin
        char *t8 = strtok(NULL, "-"); // Mes Fin
        char *t9 = strtok(NULL, "-"); // Día Fin
        if (t1 && t2 && t3 && t4 && t5 && t6 && t7 && t8 && t9) {
            strcpy(cursos[num_cursos].codigo, t1);
            strcpy(cursos[num_cursos].materia, t2);
            strcpy(cursos[num_cursos].profesor_cc, t3);

            // Reconstruir fechas
            snprintf(cursos[num_cursos].fecha_inicio, sizeof(cursos[num_cursos].fecha_inicio), "%s-%s-%s", t4, t5, t6);
            snprintf(cursos[num_cursos].fecha_fin, sizeof(cursos[num_cursos].fecha_fin), "%s-%s-%s", t7, t8, t9);
            resto = strtok(NULL, "");
            cursos[num_cursos].num_estudiantes = 0;
            if (resto && strlen(resto) > 0) {
                char *estudiante = strtok(resto, "/");
                while (estudiante && cursos[num_cursos].num_estudiantes < MAX_ESTUDIANTES) {
                    strcpy(cursos[num_cursos].estudiantes[cursos[num_cursos].num_estudiantes], estudiante);
                    cursos[num_cursos].num_estudiantes++;
                    estudiante = strtok(NULL, "/");
                }
            }

            num_cursos++;
        } else {
            printf("ERROR: Formato incorrecto en la línea: %s\n", linea);
        }
    }

    fclose(f);
}

void guardarCursosEnArchivo(const char *archivo) {
    FILE *f = fopen(archivo, "w");
    if (!f) {
        printf("No se pudo guardar en el archivo de cursos.\n");
        return;
    }

    for (int i = 0; i < num_cursos; i++) {
        fprintf(f, "%s-%s-%s-%s-%s",
                cursos[i].codigo,
                cursos[i].materia,
                cursos[i].profesor_cc,
                cursos[i].fecha_inicio,
                cursos[i].fecha_fin);

        if (cursos[i].num_estudiantes > 0) {
            fprintf(f, "-");
            for (int j = 0; j < cursos[i].num_estudiantes; j++) {
                fprintf(f, "%s", cursos[i].estudiantes[j]);
                if (j < cursos[i].num_estudiantes - 1) {
                    fprintf(f, "/");
                }
            }
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

void crearCurso() {
    if (num_cursos >= MAX_CURSOS) {
        printf("No hay espacio para mas cursos.\n");
        return;
    }

    Curso nuevoCurso;

    printf("\nIngrese el codigo del curso: ");
    fgets(nuevoCurso.codigo, sizeof(nuevoCurso.codigo), stdin);
    nuevoCurso.codigo[strcspn(nuevoCurso.codigo, "\n")] = '\0';

    if (buscarCursoPorCodigo(nuevoCurso.codigo) != -1) {
        printf("Ya existe un curso con ese codigo.\n");
        return;
    }

    printf("\nIngrese la materia: ");
    fgets(nuevoCurso.materia, sizeof(nuevoCurso.materia), stdin);
    nuevoCurso.materia[strcspn(nuevoCurso.materia, "\n")] = '\0';

    while (buscarMateriaPorCodigo(nuevoCurso.materia) == -1) {
        printf("El codigo de materia no existe. Intente nuevamente.\n");
        printf("\nIngrese la materia: ");
        fgets(nuevoCurso.materia, sizeof(nuevoCurso.materia), stdin);
        nuevoCurso.materia[strcspn(nuevoCurso.materia, "\n")] = '\0';
    }

    {
        char cc[20];
        int profesor_encontrado = 0;
        while (!profesor_encontrado) {
            printf("\nIngrese la C.C. del profesor: ");
            fgets(cc, sizeof(cc), stdin);
            cc[strcspn(cc, "\n")] = '\0';

            if (buscarProfesorPorCC(cc) == -1) {
                printf("La C.C. no existe o es invalida. Intente nuevamente.\n");
            } else {
                strcpy(nuevoCurso.profesor_cc, cc);
                profesor_encontrado = 1;
            }
        }
    }

    {
        int fechaInicioValida = 0;
        while (!fechaInicioValida) {
            printf("\nIngrese la fecha de inicio (YYYY-MM-DD): ");
            fgets(nuevoCurso.fecha_inicio, sizeof(nuevoCurso.fecha_inicio), stdin);
            nuevoCurso.fecha_inicio[strcspn(nuevoCurso.fecha_inicio, "\n")] = '\0';

            if (strlen(nuevoCurso.fecha_inicio) == 0) {
                printf("La fecha de inicio no puede estar vacia. Intente nuevamente.\n");
            } else {
                fechaInicioValida = 1;
            }
            getchar();
        }
    }

    {
        int fechaFinValida = 0;
        while (!fechaFinValida) {
            printf("\nIngrese la fecha de fin (YYYY-MM-DD): ");
            fgets(nuevoCurso.fecha_fin, sizeof(nuevoCurso.fecha_fin), stdin);
            nuevoCurso.fecha_fin[strcspn(nuevoCurso.fecha_fin, "\n")] = '\0';

            if (compararFechas(nuevoCurso.fecha_inicio, nuevoCurso.fecha_fin) > 0) {
                printf("La fecha de inicio no puede ser mayor que la fecha de fin. Intente nuevamente.\n");
            } else if (strlen(nuevoCurso.fecha_fin) == 0) {
                printf("La fecha de fin no puede estar vacia. Intente nuevamente.\n");
            } else {
                fechaFinValida = 1;
            }
            getchar();
        }
    }

    {
        char buffer[32];
        int num_est = 0;
        printf("\nIngrese el numero de estudiantes (maximo 30): ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &num_est);

        if (num_est > 30) {
            printf("No puede haber mas de 30 estudiantes.\n");
            return;
        }
        nuevoCurso.num_estudiantes = num_est;
    }

    for (int i = 0; i < nuevoCurso.num_estudiantes; i++) {
        int matricula_valida = 0;
        while (!matricula_valida) {
            char matricula[30];
            printf("Ingrese la matricula del estudiante %d: ", i + 1);
            fgets(matricula, sizeof(matricula), stdin);
            matricula[strcspn(matricula, "\n")] = '\0';

            if (buscarEstudiantePorMat(matricula) != -1) {
                strcpy(nuevoCurso.estudiantes[i], matricula);
                matricula_valida = 1;
            } else {
                printf("La matricula no existe. Intente nuevamente.\n");
            }
        }
    }

    cursos[num_cursos] = nuevoCurso;
    num_cursos++;
    printf("Curso creado con exito.\n");
}
void editarCurso() {
    char codigo[10];
    printf("\nIngrese el codigo del curso que desea editar: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    int indice = buscarCursoPorCodigo(codigo);
    if (indice == -1) {
        printf("No se encontro un curso con el codigo %s.\n", codigo);
        return;
    }

    Curso *curso = &cursos[indice];

    char fechaHoy[] = "2024-12-26"; // Decidi usar una fecha de ejemplo ya que la libreria que intente usar no me valio en el codigo.

    if (compararFechas(fechaHoy, curso->fecha_inicio) >= 0) {
        printf("No se puede editar un curso cuya fecha de inicio ya paso o es hoy.\n");
        return;
    }

    printf("\nEdicion de curso (la fecha de inicio aun no ha llegado).\n");

    {
        char nuevoCodigo[10];
        printf("\nIngrese el nuevo codigo (o deje en blanco para no cambiarlo): ");
        fgets(nuevoCodigo, sizeof(nuevoCodigo), stdin);
        nuevoCodigo[strcspn(nuevoCodigo, "\n")] = '\0';

        if (strlen(nuevoCodigo) > 0) {
            if (buscarCursoPorCodigo(nuevoCodigo) != -1 && strcmp(nuevoCodigo, curso->codigo) != 0) {
                printf("Ya existe otro curso con ese codigo. No se actualizara.\n");
            } else {
                strcpy(curso->codigo, nuevoCodigo);
                printf("Codigo actualizado.\n");
            }
        }
    }

    {
        char nuevaMateria[30];
        printf("\nIngrese la nueva materia (o deje en blanco para no cambiarla): ");
        fgets(nuevaMateria, sizeof(nuevaMateria), stdin);
        nuevaMateria[strcspn(nuevaMateria, "\n")] = '\0';

        if (strlen(nuevaMateria) > 0) {
            if (buscarMateriaPorCodigo(nuevaMateria) == -1) {
                printf("La materia no existe. No se actualizara.\n");
            } else {
                strcpy(curso->materia, nuevaMateria);
                printf("Materia actualizada.\n");
            }
        }
    }

    {
        char cc[20];
        printf("\nIngrese la nueva C.C. del profesor (o deje en blanco para no cambiarla): ");
        fgets(cc, sizeof(cc), stdin);
        cc[strcspn(cc, "\n")] = '\0';

        if (strlen(cc) > 0) {
            if (buscarProfesorPorCC(cc) == -1) {
                printf("No existe un profesor con esa C.C. No se actualizara.\n");
            } else {
                strcpy(curso->profesor_cc, cc);
                printf("Profesor actualizado.\n");
            }
        }
    }

    {
        char nuevaFechaInicio[11];
        printf("\nIngrese la nueva fecha de inicio (YYYY-MM-DD): ");
        fgets(nuevaFechaInicio, sizeof(nuevaFechaInicio), stdin);
        nuevaFechaInicio[strcspn(nuevaFechaInicio, "\n")] = '\0';

        if (strlen(nuevaFechaInicio) > 0) {
            if (compararFechas(fechaHoy, nuevaFechaInicio) >= 0) {
                printf("La nueva fecha de inicio no puede ser anterior o igual a la fecha de hoy(No se actualizara).\n");
            } else {
                strcpy(curso->fecha_inicio, nuevaFechaInicio);
                printf("Fecha de inicio actualizada.\n");
            }
        }
	getchar();
    }

    {
        char nuevaFechaFin[11];
        printf("\nIngrese la nueva fecha de fin (YYYY-MM-DD): ");
        fgets(nuevaFechaFin, sizeof(nuevaFechaFin), stdin);
        nuevaFechaFin[strcspn(nuevaFechaFin, "\n")] = '\0';

        if (strlen(nuevaFechaFin) > 0) {
            if (compararFechas(curso->fecha_inicio, nuevaFechaFin) > 0) {
                printf("La fecha de inicio no puede ser mayor que la fecha de fin. No se actualizara.\n");
            } else {
                strcpy(curso->fecha_fin, nuevaFechaFin);
                printf("Fecha de fin actualizada.\n");
            }
        }
    }

    {
        int opcionEstudiantes;
        printf("\nDesea modificar la lista de estudiantes? (1 = si, 0 = no): ");
        scanf("%d", &opcionEstudiantes);
        getchar();

        if (opcionEstudiantes == 1) {
            int num_est;
            printf("\nIngrese el numero de estudiantes (maximo 30): ");
            scanf("%d", &num_est);
            getchar();

            if (num_est > 30) {
                printf("No puede haber mas de 30 estudiantes. Se omite la actualizacion.\n");
            } else {
                char estudiantes_temp[MAX_ESTUDIANTES][30];
                int estudiantes_validados = 0;

                for (int i = 0; i < num_est; i++) {
                    printf("Ingrese la matricula del estudiante %d: ", i + 1);
                    fgets(estudiantes_temp[estudiantes_validados], sizeof(estudiantes_temp[0]), stdin);
                    estudiantes_temp[estudiantes_validados][strcspn(estudiantes_temp[estudiantes_validados], "\n")] = '\0';

                    if (buscarEstudiantePorMat(estudiantes_temp[estudiantes_validados]) == -1) {
                        printf("El estudiante %s no existe. No se guardara.\n", estudiantes_temp[estudiantes_validados]);
                    } else {
                        estudiantes_validados++;
                    }
                }

                curso->num_estudiantes = estudiantes_validados;
                for (int i = 0; i < estudiantes_validados; i++) {
                    strcpy(curso->estudiantes[i], estudiantes_temp[i]);
                }

                printf("Lista de estudiantes actualizada con %d estudiantes validos.\n", estudiantes_validados);
            }
        }
    }

    printf("\nLos cambios se han realizado. Recuerde guardar los datos en el archivo si es necesario.\n");
}

int cursoEstaEnProceso(const Curso *c) {
    char fechaHoy[] = "2024-12-26";
    if (compararFechas(c->fecha_inicio, fechaHoy) <= 0 &&
        compararFechas(fechaHoy, c->fecha_fin) < 0) {
        return 1;
    }
    return 0;
}
