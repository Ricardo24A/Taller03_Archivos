#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "curso.h"
#include "profesor.h"

void showMenu(){
  int decision;
  do{
    printf("\n ***MENU PRINCIPAL*** \n 1. Materia \n 2. Estudiante \n 3. Profesor \n 4. Curso \n 5. Salir \n");
    printf("\n Ingresa la opción deseada: ");
    scanf("%d", &decision);

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

}

void bEstudiante(){

}

void cProfesor(){
    int accion;
    do{
        printf("\n** Opciones de PROFESORES **\n 1. Crear registro Profesor \n 2. Editar registro Profesor \n 3. Retroceder \n");
        printf("Ingresa la opcion deseada: ");
        scanf("%d", &accion);
        switch(accion){
            case 1:
                crearProfesor();
                break;
            case 2:
                editarProfesor();
                break;
            case 3:
                printf("Volviendo al menu principal...");
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
        switch(accion){
            case 1:
                crearCurso();
                break;
            case 2:
                editarCurso();
                break;
            case 3:
                printf("Volviendo al menu principal...");
                break;
            default:
                printf("Opcion no valida. Intenta de nuevo. \n");
        }
    } while (accion != 3);
    return;
}

void salida(){
  //funcion para actualizar todos los archivos antes de cerrar el programa
  
  printf("\nSaliendo del programa. Hasta la próxima! \n");
}
