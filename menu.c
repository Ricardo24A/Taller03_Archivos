#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "curso.h"
#include "profesor.h"
#include "estudiante.h"
#include "materia.h"

void showMenu(){
  int decision;

  // Cargar los datos al inicio
  cargarMateriasDesdeArchivo("materias.txt");
  cargarEstudiantesDesdeArchivo("estudiantes.txt");
  cargarProfesoresDesdeArchivo("profesores.txt");
  cargarCursosDelArchivo("cursos.txt");

  do{
    printf("\n ***MENU PRINCIPAL*** \n 1. Materia \n 2. Estudiante \n 3. Profesor \n 4. Curso \n 5. Salir \n");
    printf("\n Ingresa la opcion deseada: ");
    scanf("%d", &decision);
    getchar();  // Limpiar el buffer de entrada

    switch (decision){
      case 1:
        aMateria();
        break;
      case 2:
        bEstudiante();
        break;
      case 3:
        cProfesor();
        break;
      case 4:
        dCurso();
        break;
      case 5:
        eSalir();
        break;
      default:
        printf("Opcion no valida. Intenta de nuevo. \n");
    }
  } while (decision != 5);
  return;
}

void aMateria(){
  int accion;
  do{
    printf("\n** Opciones de MATERIAS **\n 1. Crear Materia \n 2. Editar Materia \n 3. Retroceder \n");
    printf("Ingresa la opcion deseada: ");
    scanf("%d", &accion);
    getchar();  // Limpiar el buffer de entrada

    switch(accion){
        case 1:
            crearMateria();
            break;
        case 2:
            editarMateria();
            break;
        case 3:
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opcion no valida. Intenta de nuevo. \n");
    }
  } while (accion != 3);
  return;
}

void bEstudiante(){
  int accion;
  do{
    printf("\n** Opciones de ESTUDIANTES **\n 1. Crear registro Estudiante \n 2. Editar registro Estudiante \n 3. Retroceder \n");
    printf("Ingresa la opcion deseada: ");
    scanf("%d", &accion);
    getchar();  // Limpiar el buffer de entrada

    switch(accion){
        case 1:
            crearEstudiante();
            break;
        case 2:
            editarEstudiante();
            break;
        case 3:
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opcion no valida. Intenta de nuevo. \n");
    }
  } while (accion != 3);
  return;
}

void cProfesor(){
  int accion;
  do{
    printf("\n** Opciones de PROFESORES **\n 1. Crear registro Profesor \n 2. Editar registro Profesor \n 3. Retroceder \n");
    printf("Ingresa la opcion deseada: ");
    scanf("%d", &accion);
    getchar();  // Limpiar el buffer de entrada

    switch(accion){
        case 1:
            crearProfesor();
            break;
        case 2:
            editarProfesor();
            break;
        case 3:
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opcion no valida. Intenta de nuevo. \n");
    }
  } while (accion != 3);
  return;
}

void dCurso(){
  int accion;
  do{
    printf("\n** Opciones de CURSOS **\n 1. Crear Curso \n 2. Editar Curso \n 3. Retroceder \n");
    printf("Ingresa la opcion deseada: ");
    scanf("%d", &accion);
    getchar();  // Limpiar el buffer de entrada

    switch(accion){
        case 1:
            crearCurso();
            break;
        case 2:
            editarCurso();
            break;
        case 3:
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opcion no valida. Intenta de nuevo. \n");
    }
  } while (accion != 3);
  return;
}

// Función para guardar los cambios antes de salir
void eSalir(){
  // Guardar todos los datos antes de salir
  guardarMateriasEnArchivo("materias.txt");
  guardarEstudiantesEnArchivo("estudiantes.txt");
  guardarProfesoresEnArchivo("profesores.txt");
  guardarCursosEnArchivo("cursos.txt");

  // Mensaje final
  printf("\nSaliendo del programa. Hasta la proxima! \n");
}
