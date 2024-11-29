#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

//variable global para almacenar el dataframe activo
Dataframe *dataframeActivo = NULL;
int indiceDataframe = -1;

void mostrarPrompt() {
    if (dataframeActivo != NULL) {
        printf("[df%d: %d,%d]:> ", indiceDataframe, dataframeActivo->numFilas, dataframeActivo->numColumnas);
    } else {
        printf("[?]:> ");
    }
    fflush(stdout);
}


int main (int argc,char *argv[]){
    printf("DATOS DEL ALUMNO:\n\n");
    printf("NOMBRE + APELLIDOS: \tJamal Menchi Hajji\n\n");
    printf("CORREO:\tjamal.menchi@goumh.umh.es\n\n");

    char comando[256];
    // char nombreColumna[100];
    char nombreFichero[100];
    // int ascendente = 1;
    int numFilas = 10;

    while (1) {
        // Mostrar el prompt
        mostrarPrompt();

        fgets(comando, sizeof(comando), stdin);

         // Eliminar el salto de línea '\n' al final del comando
        comando[strcspn(comando, "\n")] = 0;

        // Comando quit: Finalizar el programa
        if (strcmp(comando, "quit") == 0) {
            printf("\033[0;32mEXIT PROGRAM\033[0m\n");
            liberarMemoria();  // Liberar toda la memoria reservada
            break;
        }

        // Comando load: Cargar un fichero CSV
        else if (strncmp(comando, "load", 4) == 0) {
            // Obtener el nombre del fichero
            if (sscanf(comando + 5, "%s", nombreFichero) == 1) {
                dataframeActivo = cargarCSV(nombreFichero);
                if (dataframeActivo) {
                    printf("\033[0;32mArchivo %s cargado correctamente.\033[0m\n", nombreFichero);
                    indiceDataframe++;
                } else {
                    printf("\033[0;31mError al cargar el archivo %s.\033[0m\n", nombreFichero);
                }
            } else {
                printf("\033[0;31mDebe especificar un nombre de fichero.\033[0m\n");
            }
        }

        // Comando view: Mostrar las primeras n filas (o 10 por defecto)
        else if (strncmp(comando, "view", 4) == 0) {
            if (dataframeActivo != NULL) {
                if (sscanf(comando + 5, "%d", &numFilas) == 1) {
                    if (numFilas < 0) {
                        printf("\033[0;31mEl numero de filas debe ser mayor que 0.\033[0m\n");
                    } else {
                        viewDataframe(dataframeActivo, numFilas);
                    }
                } else {
                    // Si no se especifica número, usar 10 por defecto
                    viewDataframe(dataframeActivo, 10);
                }
            } else {
                printf("\033[0;31mNo hay un dataframe activo para mostrar.\033[0m\n");
            }
        }

        // Comando meta: mostrar los metadatos del dataframe
        else if (strcmp(comando, "meta") == 0) {
            if (dataframeActivo) {
                mostrarMetadatos(dataframeActivo);
            } else {
                printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n"); // Mensaje de error
            }
        }

        // // Comando sort: ordenar dataframe
        // else if (strncmp(comando, "sort", 4) == 0) {
        //     char nombreColumna[100], orden[4] = "asc";
        //     if (sscanf(comando + 5, "%s %s", nombreColumna, orden) >= 1) {
        //         int ascendente = (strcmp(orden, "des") == 0) ? : 1;
        //         ordenarDataframe(dataframeActivo, nombreColumna, ascendente);
        //     } else {
        //                 printf("\033[0;31mError: número de parámetros incorrecto.\033[0m\n");
        //     }
        // }
       
        
        // // Comando save: guardar dataframe
        // else if (strncmp(comando, "save", 4) == 0) {
        //     char nombreFichero[100];
        //     if (sscanf(comando + 5, "%s", nombreFichero) == 1) {
        //         guardarCSV(dataframeActivo, nombreFichero);
        //     } else {
        //         printf("\033[0;31mError: debe especificar un nombre de fichero.\033[0m\n");
        //     }
        // }


        // Si el comando no es válido, mostrar un mensaje de error
        else {
            printf("\033[0;31mComando no valido.\033[0m\n");
        }
    }
    return 0;
}