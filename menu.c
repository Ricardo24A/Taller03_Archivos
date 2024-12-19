#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

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
        printf("Opciones no valida. Intenta de nuevo. \n");
    }
  } while (decision != 5);
  return;
}

void aMateria(){

}

void bEstudiante(){

}

void cProfesor(){

}

void dCurso(){

}

void salida(){
  //funcion para actualizar todos los archivos antes de cerrar el programa
  
  printf("\nSaliendo del programa. Hasta la próxima! \n");
}
