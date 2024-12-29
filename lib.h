#ifndef LIB_H
#define LIB_H

// *************** TIPOS DE DATOS ***************

// Tipo enumerado para representar los diferentes tipos de datos en las columnas
typedef enum {
    TEXTO,
    NUMERICO,
    FECHA
} TipoDato;

// Estructura para representar una columna del dataframe
typedef struct {
    char nombre[30];         // Nombre de la columna
    TipoDato tipo;           // Tipo de datos de la columna (TEXTO, NUMERICO, FECHA)
    void *datos;             // Puntero genérico para almacenar los datos de la columna
    unsigned char *esNulo;   // Array paralelo, indica valores nulos (1: nulo, 0: no nulo)
    int numFilas;            // Número de filas en la columna
} Columna;

// Estructura para representar el dataframe como un conjunto de columnas
typedef struct {
    Columna *columnas;       // Array de columnas (con tipos de datos distintos)
    int numColumnas;         // Número de columnas en el dataframe
    int numFilas;            // Número de filas (igual para todas las columnas)
    int *indice;             // Array para ordenar las filas
    char nombre[51]; // Nombre del dataframe
} Dataframe;

// Alias para tipos FECHA: 'Fecha' alias de 'struct tm' (#include <time.h>)
typedef struct tm Fecha;

// Estructura para representar un nodo de la lista
typedef struct NodoLista {
    Dataframe *df;           // Puntero a un dataframe
    struct NodoLista *siguiente; // Puntero al siguiente nodo de la lista
} Nodo;

// Estructura para representar la lista de dataframes
typedef struct {
    int numDFs;              // Número de dataframes almacenados en la lista
    Nodo *primero;           // Puntero al primer nodo de la lista
} Lista;

// *************** FUNCIONES PRINCIPALES ***************

// Funciones para el flujo de datos (main)
void mostrarPromptDataframe();             // Mostrar el prompt basado en el estado actual
void inicializarPrograma();                // Inicializar el programa y mostrar información inicial
void manejarComandoLoad(const char *arg);  // Manejar el comando 'load'
void manejarComandoView(const char *arg);  // Manejar el comando 'view'
void manejarComandoMeta();                 // Manejar el comando 'meta'
void manejarComandoSort(const char *cmd);  // Manejar el comando 'sort'
void manejarComandoSave(const char *cmd);  // Manejar el comando 'save'
void manejarComandoFilter(const char *cmd);// Manejar el comando 'filter'
void manejarComandoDelNull(const char *cmd);// Manejar el comando 'delnull'
void manejarComandoDelColum(const char *cmd);// Manejar el comando 'delcolum'
void manejarComandoQuarter(const char *cmd);// Manejar el comando 'quarter'
void manejarComandoDesconocido();          // Manejar comandos no válidos
void manejarComandoName(const char *argumento);
void manejarComandoList(); // Mostrar listado de dataframes cargados
void manejarComandoPrefix(const char *argumento);
void finalizarPrograma();                  // Liberar memoria y finalizar el programa
void ejecutarCicloComandos();              // Ciclo principal para manejar comandos

// *************** FUNCIONES AUXILIARES ***************
char *strndup(const char *str, size_t n);

// Gestión de memoria
void liberarColumna(Columna *columna);     // Liberar memoria asociada a una columna
void liberarDataframe(Dataframe *df);      // Liberar memoria asociada a un dataframe
void liberarMemoriaLista();                // Liberar memoria de la lista de dataframes
void liberarMemoria();                     // Liberar toda la memoria dinámica

// Validación de datos
int contarFilas(FILE *archivo);            // Contar filas en un archivo CSV
int esNumerico(const char *str);           // Validar si una cadena es numérica
int esBisiesto(int anio);                  // Validar si un año es bisiesto
int esFecha(char *str);                    // Validar si una cadena es una fecha
int esTexto(char *str);                    // Validar si una cadena es texto

// Funciones de carga y manejo de datos
Dataframe* cargarCSV(char *nombreFichero); // Cargar datos desde un archivo CSV
void viewDataframe(Dataframe *df, int n);  // Visualizar las primeras 'n' filas de un dataframe
void mostrarMetadatos(Dataframe *df);      // Mostrar metadatos de un dataframe
void sortDataframe(Dataframe *df, const char *cmd); // Ordenar filas en un dataframe
void saveDataframe(Dataframe *df, const char *nombreFichero); // Guardar dataframe a CSV
void filterDataframe(Dataframe *df, const char *cmd); // Filtrar filas en un dataframe
void delNull(Dataframe *df, const char *col); // Eliminar filas con valores nulos
void delColumn(Dataframe *df, const char *col); // Eliminar una columna
void quarterColumn(Dataframe *df, const char *col, const char *newCol); // Crear columna de trimestre

// Funciones internas estáticas
static int contarColumnas(FILE *archivo);  // Contar columnas en un archivo CSV
static void leerEncabezados(FILE *archivo, Dataframe *df); // Leer encabezados de un archivo CSV
static void determinarTipos(FILE *archivo, Dataframe *df);// Determinar tipos de datos de las columnas
static void inicializarDataframe(Dataframe *df, int numCols); // Inicializar un dataframe
static void leerDatos(FILE *archivo, Dataframe *df);        // Leer datos de un archivo CSV

#endif // LIB_H
