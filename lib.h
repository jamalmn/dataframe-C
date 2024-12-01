#ifndef LIB_H
#define LIB_H
// Tipo enumerado para representar los diferentes tipos de datos en las columnas
typedef enum
{
    TEXTO,
    NUMERICO,
    FECHA
} TipoDato;
// Estructura para representar una columna del dataframe
typedef struct
{
    char nombre[30];
    // Nombre de la columna
    TipoDato tipo;
    // Tipo de datos de la columna (TEXTO, NUMERICO, FECHA)
    void *datos;
    // Puntero genérico para almacenar los datos de la columna
    unsigned char *esNulo; // Array paralelo, indica valores nulos (1/0: nulo/noNulo)
    int numFilas;
    // Número de filas en la columna
} Columna;
// Estructura para representar el dataframe como un conjunto de columnas
typedef struct
{
    Columna *columnas;
    // Array de columnas (con tipos de datos distintos)
    int numColumnas;
    int numFilas;
    int *indice;
} Dataframe;
// Número de columnas en el dataframe
// Número de filas (igual para todas las columnas)
// Array para ordenar las filas
// Alias para tipos FECHA: 'Fecha' alias de 'struct tm' (#include <time.h>)
typedef struct tm Fecha;

// Estructura para representar un nodo de la lista
typedef struct NodoLista
{
    Dataframe *df;
    // Puntero a un dataframe
    struct NodoLista *siguiente; // Puntero al siguiente nodo de la lista
} Nodo;
// Estructura para representar la lista de Dataframe’s
typedef struct
{
    int numDFs;
    // Número de dataframes almacenados en la lista
    Nodo *primero;
} Lista;

void liberarColumna(Columna *columna);
void liberarDataframe(Dataframe *df);
void liberarMemoriaLista();
void liberarMemoria();

int esNumerico(char *str);
int esBisiesto(int anio);
int esFecha(char *str);
int esTexto(char *str);
Dataframe* cargarCSV( char *nombreFichero);
void viewDataframe(Dataframe *df, int n);
void mostrarMetadatos(Dataframe *df);

#endif
