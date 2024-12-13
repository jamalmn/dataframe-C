#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "lib.h"

// *******************************************************
// *******************************************************
// ******* FUNCIONES PARA EL FLUJO DE DATOS (MAIN)********
// *******************************************************

// Puntero global al Dataframe activo. Inicialmente es NULL hasta que se cargue un dataframe.
// Esto permite acceder al dataframe actualmente en uso en todo el programa.
Dataframe *dataframeActivo = NULL;

// Índice del dataframe activo. Inicialmente es -1, indicando que no hay dataframe cargado.
// A medida que se cargan nuevos dataframes, este índice se incrementa para hacer referencia al dataframe actual.
int indiceDataframe = -1;

// Mostrar el prompt personalizado basado en el estado del programa
void mostrarPromptDataframe() {
    if (dataframeActivo != NULL) {
        printf("[df%d: %d filas, %d columnas]:> ", indiceDataframe, dataframeActivo->numFilas, dataframeActivo->numColumnas);
    } else {
        printf("[?]:> ");
    }
    fflush(stdout);
}

// Inicialización del programa
void inicializarPrograma() {
    printf("\033[1;34m=== PROGRAMA DE MANEJO DE DATAFRAMES ===\033[0m\n\n");
    printf("\033[1;32mDATOS DEL ALUMNO:\033[0m\n");
    printf("NOMBRE + APELLIDOS: Jamal Menchi Hajji\n");
    printf("CORREO: jamal.menchi@goumh.umh.es\n\n");
}

// Manejar el comando "load"
void manejarComandoLoad(const char *argumento) {
    char nombreFichero[100];
    if (sscanf(argumento, "%s", nombreFichero) == 1) {
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

// Manejar el comando "view"
void manejarComandoView(const char *argumento) {
    int numFilas = 10;
    if (dataframeActivo != NULL) {
        if (sscanf(argumento, "%d", &numFilas) == 1) {
            if (numFilas <= 0) {
                printf("\033[0;31mEl número de filas debe ser mayor que 0.\033[0m\n");
            } else {
                viewDataframe(dataframeActivo, numFilas);
            }
        } else {
            viewDataframe(dataframeActivo, 10); // Valor predeterminado
        }
    } else {
        printf("\033[0;31mNo hay un dataframe activo para mostrar.\033[0m\n");
    }
}

// Manejar el comando "meta"
void manejarComandoMeta() {
    if (dataframeActivo) {
        mostrarMetadatos(dataframeActivo);
    } else {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
    }
}

// Manejar el comando "sort"
void manejarComandoSort(const char *comando) {
    char nombreColumna[100];
    char orden[4] = "asc";  // Por defecto, ascendente
    int numParametros = sscanf(comando, "%s %s", nombreColumna, orden);

    if (numParametros < 1 || numParametros > 2) {
        printf("\033[0;31mError: número incorrecto de parámetros.\033[0m\n");
        return;
    }

    // Validar que la columna existe y ejecutar la función
    sortDataframe(dataframeActivo, comando);
}

// Manejar el comando "save"
void manejarComandoSave(const char *comando) {
    char nombreFichero[100];
    if (sscanf(comando, "%s", nombreFichero) == 1) {
        saveDataframe(dataframeActivo, nombreFichero);
    } else {
        printf("\033[0;31mError: debe especificar el nombre del archivo.\033[0m\n");
    }
}
// Manejar el comando "filter"
void manejarComandoFilter(const char *comando) {
    // Lógica para procesar el comando filter, como obtener la columna, operador y valor
    filterDataframe(dataframeActivo, comando);
}

// Manejar el comando "delnull"
void manejarComandoDelNull(const char *comando) {
    // Obtener el nombre de la columna y eliminar las filas con valores nulos
    delNull(dataframeActivo, comando);
}


// Manejar el comando "delcolum"
void manejarComandoDelColum(const char *comando) {
    // Obtener el nombre de la columna y eliminarla del dataframe
    delColumn(dataframeActivo, comando);
}

// // Manejar el comando "quarter"
// void manejarComandoQuarter(const char *comando) {
//     char nombreColumna[100];
//     char nombreNuevaColumna[100];

//     if (sscanf(comando, "%s %s", nombreColumna, nombreNuevaColumna) == 2) {
//         quarterColumn(dataframeActivo, nombreColumna, nombreNuevaColumna);
//     } else {
//         printf("\033[0;31mError: parámetros incorrectos para quarter.\033[0m\n");
//     }
// }

// Manejar comandos desconocidos
void manejarComandoDesconocido() {
    printf("\033[0;31mComando no válido.\033[0m\n");
}

// Liberar memoria y finalizar
void finalizarPrograma() {
    printf("\033[0;32mEXIT PROGRAM\033[0m\n");
    liberarMemoria();
}

// Ciclo principal para manejar los comandos del usuario
void ejecutarCicloComandos() {
    char comando[256];
    while (1) {
        mostrarPromptDataframe();

        // Leer comando del usuario
        fgets(comando, sizeof(comando), stdin);

        // Eliminar el salto de línea '\n' al final del comando
        comando[strcspn(comando, "\n")] = 0;

        // Manejar comandos específicos
        if (strcmp(comando, "quit") == 0) {
            finalizarPrograma();
            break;
        } else if (strncmp(comando, "load", 4) == 0) {
            manejarComandoLoad(comando + 5);
        } else if (strncmp(comando, "view", 4) == 0) {
            manejarComandoView(comando + 5);
        } else if (strcmp(comando, "meta") == 0) {
            manejarComandoMeta();
        }  // Comando sort
        else if (strncmp(comando, "sort", 4) == 0) {
            manejarComandoSort(comando + 5);  // Pasar el comando sin "sort"
        }
        else if (strncmp(comando, "save", 4) == 0) {
            manejarComandoSave(comando + 5);  // Pasar el comando sin "save"
        }
        else if (strncmp(comando, "filter", 6) == 0) {
            manejarComandoFilter(comando + 7);  // Pasar el comando sin "filter"
        }
        // Comando delnull
        else if (strncmp(comando, "delnull", 7) == 0) {
            manejarComandoDelNull(comando + 8);  // Pasar el comando sin "delnull"
        }
        // Comando delcolum
        else if (strncmp(comando, "delcolum", 8) == 0) {
            manejarComandoDelColum(comando + 9);  // Pasar el comando sin "delcolum"
        }
        // // Comando quarter
        // else if (strncmp(comando, "quarter", 7) == 0) {
        //     manejarComandoQuarter(comando + 8);  // Pasar el comando sin "quarter"
        // } 
        else {
            manejarComandoDesconocido();
        }
    }
}

// ********************************************************
// FUNCIONES PARA LA LOGICA DE LOS COMANDOS ***************
// ********************************************************

// estructura global para manejar la lista de dataframes
Lista listaDataframes = {0, NULL};

// gestion de la memoria dinamica
// liberar memoria asociada a una columna
void liberarColumna(Columna *columna)
{
    if (columna)
    {
        // liberar los datos almacenados en la columna segun su tipo
        if (columna->datos)
        {
            free(columna->datos);
        }
        // liberar el array esNulo
        if (columna->esNulo)
        {
            free(columna->esNulo);
        }
    }
}
// liberar memoria asociada a un dataframe
void liberarDataframe(Dataframe *df)
{
    if (df)
    {
        // liberar las columnas
        for (int i = 0; i < df->numColumnas; i++)
        {
            liberarColumna(&(df->columnas[i]));
        }
        // liberar el array de columnas
        if (df->columnas)
        {
            free(df->columnas);
        }
        // liberar el array de indices
        if (df->indice)
        {
            free(df->indice);
        }
        // liberar estructura dataframe
        free(df);
    }
}
// liberar toda la lista de dataframes
void liberarMemoriaLista()
{
    Nodo *actual = listaDataframes.primero;
    while (actual)
    {
        Nodo *temp = actual;
        actual = actual->siguiente; // moverse al siguiente nodo antes de liberar el actual
        // liberar dataframe almacenado en el nodo
        liberarDataframe(temp->df);
        // liberar el nodo de la lista
        free(temp);
    }
    listaDataframes.primero = NULL; // reiniciar la lista
    listaDataframes.numDFs = 0;     // reiniciar el contador de dataframes
}

// liberar toda memoria usada por el programa
void liberarMemoria()
{
    liberarMemoriaLista();
    printf("memoria liberada correctamente\n");
}

// funcion que valida si es numerico

int esNumerico(char *str)
{
    int i = 0;
    // Manejar los signos (+ o -) al principio
    if (str[0] == '-' || str[0] == '+')
    {
        i++;
    }

    // Recorrer cada carácter, asegurando que todo sea un dígito
    for (; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return 0; // No es numérico si algún carácter no es un dígito
        }
    }

    // Retorna verdadero solo si la cadena no está vacía
    return (i > 0);
}

// funcion que valida si es fecha
int esBisiesto(int anio)
{
    return (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0));
}

int esFecha(char *str)
{
    // Verificar longitud y formato básico
    if (strlen(str) != 11 || strlen(str) != 10)
    {
        if (strlen(str) == 11 && str[10] != '\n')
        {
            return 0;
        }
        else if (strlen(str) == 10)
        {
            return 0;
        }
    }
    if (str[4] != '-' || str[7] != '-')
    {
        return 0;
    }

    // Verificar que los caracteres de año, mes y día sean dígitos
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue; // Saltar los guiones
        if (!isdigit(str[i]))
        {
            return 0;
        } // Si no es dígito, no es válido
    }

    // Extraer año, mes y día
    int anio = atoi(str);
    int mes = atoi(str + 5);
    int dia = atoi(str + 8);

    // Verificar rango del mes
    if (mes < 1 || mes > 12)
        return 0;

    // Verificar rango del día dependiendo del mes
    if (dia < 1)
        return 0;

    if (mes == 2)
    { // Febrero
        if (esBisiesto(anio))
        {
            if (dia > 29)
                return 0;
        }
        else
        {
            if (dia > 28)
                return 0;
        }
    }
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
    {
        if (dia > 30)
            return 0; // Meses con 30 días
    }
    else
    {
        if (dia > 31)
            return 0; // Meses con 31 días
    }

    // Todo es válido
    return 1;
}

// Función para convertir una fecha en formato YYYY-MM-DD a struct tm
int parseFecha(const char *str, struct tm *fecha) {
    // Usamos sscanf para descomponer la fecha en año, mes y día
    int anio, mes, dia;
    if (sscanf(str, "%4d-%2d-%2d", &anio, &mes, &dia) != 3) {
        return 0;  // Error de parseo
    }

    // Asignamos los valores a la estructura tm
    fecha->tm_year = anio - 1900;  // Años desde 1900
    fecha->tm_mon = mes - 1;       // Meses de 0 a 11
    fecha->tm_mday = dia;         // Día del mes
    fecha->tm_hour = 0;
    fecha->tm_min = 0;
    fecha->tm_sec = 0;
    fecha->tm_isdst = -1; // Sin información sobre el horario de verano

    return 1; // Éxito
}


int esTexto(char *str)
{
    // Verificar si el valor está vacío
    if (str == NULL || str[0] == '\0')
    {
        return 0; // No es texto si está vacío
    }

    // Verificar si es numérico
    if (esNumerico(str))
    {
        return 0; // No es texto si es numérico
    }

    // Verificar si es fecha
    if (esFecha(str))
    {
        return 0; // No es texto si es una fecha válida
    }

    // Permitir letras, dígitos, y caracteres comunes como guiones y espacios
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && !isspace(str[i]) && str[i] != '_') {
            return 0; // No es texto si contiene caracteres no permitidos
        }
    }

    return 1; // es texto valido
}

Dataframe *cargarCSV(char *nombreFichero)
{
    FILE *archivo = fopen(nombreFichero, "r");
    if (!archivo)
    {
        printf("\033[0;31mError: no se pudo abrir el archivo %s.\033[0m\n", nombreFichero);
        return NULL;
    }

    char buffer[1024];
    int numFilas = 0;
    int numColumnas = 0;
    Dataframe *df = (Dataframe *)malloc(sizeof(Dataframe));

    // Leer 1a fila para obtener el número de columnas
    long posicion = ftell(archivo);
    if (fgets(buffer, sizeof(buffer), archivo))
    {
        char *token = strtok(buffer, ",");
        while (token)
        {
            numColumnas++;
            token = strtok(NULL, ",");
        }
        df->numColumnas = numColumnas;
        df->columnas = (Columna *)malloc(numColumnas * sizeof(Columna));
        rewind(archivo); // Volver al inicio del archivo para leer de nuevo
    }

    fseek(archivo, posicion, SEEK_SET);

    // Leer los nombres de las columnas
    if (fgets(buffer, sizeof(buffer), archivo))
    {
        int i = 0;
        char *token = strtok(buffer, ",");
        while (token)
        {
            token[strcspn(token, "\r\n")] = '\0'; // Limpiar caracteres de nueva línea
            strncpy(df->columnas[i].nombre, token, 30);
            token = strtok(NULL, ",");
            i++;
        }
    }

    posicion = ftell(archivo);

    // Leer los tipos de datos a partir de la segunda fila
    if (fgets(buffer, sizeof(buffer), archivo))
    {
        int i = 0;
        char *token = strtok(buffer, ",");
        while (token)
        {
            if (esNumerico(token))
            {
                df->columnas[i].tipo = NUMERICO;
                df->columnas[i].datos = malloc(1000 * sizeof(int));
            }
            else if (esFecha(token))
            {
                df->columnas[i].tipo = FECHA;
                df->columnas[i].datos = malloc(1000 * sizeof(char *));
            }
            else
            {
                df->columnas[i].tipo = TEXTO;
                df->columnas[i].datos = malloc(1000 * sizeof(char *));
            }
            df->columnas[i].esNulo = malloc(1000 * sizeof(unsigned char));
            token = strtok(NULL, ",");
            i++;
        }
    }

    fseek(archivo, posicion, SEEK_SET);

    // Contar filas en el archivo
    while (fgets(buffer, sizeof(buffer), archivo))
    {
        numFilas++;
    }
    df->numFilas = numFilas;

    // Inicializar el array `indice`
    df->indice = malloc(numFilas * sizeof(int));
    for (int i = 0; i < numFilas; i++) {
        df->indice[i] = i; // Inicializar con valores consecutivos
    }

    printf("[DEBUG] Número de filas: %d\n", numFilas);
    printf("[DEBUG] Índice inicializado: ");
    for (int i = 0; i < numFilas; i++) {
        printf("%d ", df->indice[i]);
    }
    printf("\n");

    // Volver a procesar el archivo para leer los datos
    fseek(archivo, posicion, SEEK_SET);
    numFilas = 0; // Reiniciar contador de filas

    while (fgets(buffer, sizeof(buffer), archivo))
    {
        int i = 0;
        char *inicio = buffer;
        char *fin;

        while ((fin = strchr(inicio, ',')) || *inicio != '\n')
        {
            if (fin)
            {
                *fin = '\0'; // Terminar la cadena en la coma
            }

            // Verificar si el valor está vacío
            if (inicio[0] == '\0')
            {
                df->columnas[i].esNulo[numFilas] = 1; // Marcar como nulo
            }
            else
            {
                // Validar tipo de dato según la columna
                if (df->columnas[i].tipo == NUMERICO && esNumerico(inicio))
                {
                    ((int *)df->columnas[i].datos)[numFilas] = atoi(inicio);
                    df->columnas[i].esNulo[numFilas] = 0;
                }
                else if (df->columnas[i].tipo == FECHA && esFecha(inicio))
                {
                    ((char **)df->columnas[i].datos)[numFilas] = strdup(inicio);
                    df->columnas[i].esNulo[numFilas] = 0;
                }
                else if (df->columnas[i].tipo == TEXTO)
                {
                    inicio[strcspn(inicio, "\r\n")] = '\0';
                    if (esTexto(inicio))
                    {
                        ((char **)df->columnas[i].datos)[numFilas] = strdup(inicio);
                        df->columnas[i].esNulo[numFilas] = 0;
                    }
                    else
                    {
                        df->columnas[i].esNulo[numFilas] = 1;
                    }
                }
                else
                {
                    df->columnas[i].esNulo[numFilas] = 1; // Tipo incorrecto
                }
            }

            if (fin)
            {
                inicio = fin + 1; // Avanzar al próximo token
            }
            else
            {
                break; // Fin de la fila
            }
            i++;
        }
        numFilas++;
    }

    fclose(archivo);
    return df;
}

// funcion para mostrar las primeras 'n' filas del dataframe
void viewDataframe(Dataframe *df, int n) {
    if (!df) {
        printf("\033[0;31mError: no hay dataframe activo.\033[0m\n");
        return;
    }

    if (n > df->numFilas)
        n = df->numFilas;

    // Mostrar los nombres de las columnas
    for (int i = 0; i < df->numColumnas; i++) {
        printf("\033[0;32m%s\t", df->columnas[i].nombre);
    }
    printf("\033[0m\n");

    // Mostrar las primeras 'n' filas respetando el índice
    for (int i = 0; i < n; i++) {
        int filaReal = df->indice[i]; // Usar el índice para acceder a las filas reales
        for (int col = 0; col < df->numColumnas; col++) {
            if (df->columnas[col].esNulo[filaReal]) {
                printf("\033[0;32m#N/A\t");
            } else {
                if (df->columnas[col].tipo == NUMERICO) {
                    printf("\033[0;32m%d\t", ((int *)df->columnas[col].datos)[filaReal]); // Mostrar enteros
                } else if (df->columnas[col].tipo == TEXTO || df->columnas[col].tipo == FECHA) {
                    printf("\033[0;32m%s\t", ((char **)df->columnas[col].datos)[filaReal]); // Mostrar texto o fecha
                }
            }
        }
        printf("\033[0m\n");
    }
}

// Función para mostrar metadatos del dataframe
void mostrarMetadatos(Dataframe *df)
{
    if (!df)
    {
        printf("\033[0;31mError: no hay dataframe activo.\033[0m\n");
        return;
    }

    printf("\033[0;32mNumero de columnas: %d\n", df->numColumnas);
    printf("Numero de filas: %d\n\033[0m", df->numFilas);

    // Mostrar metadatos de cada columna
    for (int i = 0; i < df->numColumnas; i++)
    {
        printf("\033[0;32mColumna: %s\n", df->columnas[i].nombre);
        printf("Tipo: ", df->columnas[i].tipo);
        switch (df->columnas[i].tipo)
        {
        case TEXTO:
            printf("Texto\n");
            break;
        case NUMERICO:
            printf("Numerico\n");
            break;
        case FECHA:
            printf("Fecha\n");
            break;
        }

        // Contar y mostrar valores nulos
        int nulos = 0;
        for (int j = 0; j < df->numFilas; j++)
        {
            if (df->columnas[i].esNulo[j])
            {
                nulos++;
            }
        }
        printf("Valores nulos: %d\n", nulos);
        printf("----------------------------\n");
    }
    printf("\033[0m"); // Resetear color
}

void sortDataframe(Dataframe *df, const char *comando) {
    if (!df) {
        printf("\033[0;31mError: no hay un dataframe activo.\033[0m\n");
        return;
    }

    // Validar el índice
    if (!df->indice) {
        printf("\033[0;31mError: El índice no está inicializado.\033[0m\n");
        return;
    }

    // printf("[DEBUG] Número de filas: %d, Número de columnas: %d\n", df->numFilas, df->numColumnas);

    // Variables para el comando
    char nombreColumna[30];
    char orden[4] = "asc"; // Orden predeterminado
    int columnaIndex = -1;

    // Parsear el comando
    int numParametros = sscanf(comando, "%s %s", nombreColumna, orden);
    // printf("[DEBUG] Comando recibido: columna='%s', orden='%s', parámetros=%d\n", nombreColumna, orden, numParametros);

    if (numParametros < 1 || numParametros > 2) {
        printf("\033[0;31mError: número incorrecto de parámetros.\033[0m\n");
        return;
    }

    // Validar el orden
    if (numParametros == 2 && strcmp(orden, "asc") != 0 && strcmp(orden, "des") != 0) {
        printf("\033[0;31mError: El parámetro de orden debe ser 'asc' o 'des'.\033[0m\n");
        return;
    }

    // Buscar el índice de la columna
    for (int i = 0; i < df->numColumnas; i++) {
        // printf("[DEBUG] Comparando columna '%s' con '%s'\n", df->columnas[i].nombre, nombreColumna);
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0) {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1) {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    // printf("[DEBUG] Columna encontrada en el índice %d\n", columnaIndex);

    // Mostrar índice inicial
    // printf("[DEBUG] Índice inicial: ");
    for (int i = 0; i < df->numFilas; i++) {
        printf("%d ", df->indice[i]);
    }
    printf("\n");

    // Ordenar las filas utilizando el array 'indice'
    for (int i = 0; i < df->numFilas - 1; i++) {
        for (int j = i + 1; j < df->numFilas; j++) {
            int filaI = df->indice[i];
            int filaJ = df->indice[j];
            int comparar = 0;

            // Comparar valores nulos
            if (df->columnas[columnaIndex].esNulo[filaI] && df->columnas[columnaIndex].esNulo[filaJ]) {
                comparar = 0; // Ambos nulos
                // printf("[DEBUG] Ambos valores son nulos\n");
            } else if (df->columnas[columnaIndex].esNulo[filaI]) {
                comparar = -1; // FilaI nulo, va primero
                // printf("[DEBUG] Fila %d (índice %d) es nulo\n", i, filaI);
            } else if (df->columnas[columnaIndex].esNulo[filaJ]) {
                comparar = 1; // FilaJ nulo, va primero
                // printf("[DEBUG] Fila %d (índice %d) es nulo\n", j, filaJ);
            } else {
                // Comparar según el tipo de dato
                if (df->columnas[columnaIndex].tipo == NUMERICO) {
                    int valorI = ((int *)df->columnas[columnaIndex].datos)[filaI];
                    int valorJ = ((int *)df->columnas[columnaIndex].datos)[filaJ];
                    comparar = (valorI > valorJ) - (valorI < valorJ);
                    // printf("[DEBUG] Comparando NUMERICO: %d y %d\n", valorI, valorJ);
                } else if (df->columnas[columnaIndex].tipo == TEXTO) {
                    char *valorI = ((char **)df->columnas[columnaIndex].datos)[filaI];
                    char *valorJ = ((char **)df->columnas[columnaIndex].datos)[filaJ];
                    comparar = strcmp(valorI, valorJ);
                    // printf("[DEBUG] Comparando TEXTO: '%s' y '%s'\n", valorI, valorJ);
                }
            }

            // Realizar intercambio si es necesario
            if ((strcmp(orden, "asc") == 0 && comparar > 0) || (strcmp(orden, "des") == 0 && comparar < 0)) {
                int temp = df->indice[i];
                df->indice[i] = df->indice[j];
                df->indice[j] = temp;
                // printf("[DEBUG] Intercambiando índices: %d y %d\n", i, j);
            }
        }
    }

    // Mostrar índice final
    // printf("[DEBUG] Índice final: ");
    for (int i = 0; i < df->numFilas; i++) {
        printf("%d ", df->indice[i]);
    }
    printf("\n");

    printf("\033[0;32mEl dataframe ha sido ordenado por la columna '%s' en orden %s.\033[0m\n", nombreColumna, orden);
}

void saveDataframe(Dataframe *df, const char *nombreFichero) {
    if (!df) {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    FILE *archivo = fopen(nombreFichero, "w");
    if (!archivo) {
        printf("\033[0;31mError: No se pudo crear el archivo '%s'.\033[0m\n", nombreFichero);
        return;
    }

    // Escribir los nombres de las columnas
    for (int i = 0; i < df->numColumnas; i++) {
        fprintf(archivo, "%s", df->columnas[i].nombre);
        if (i < df->numColumnas - 1) {
            fprintf(archivo, ",");
        }
    }
    fprintf(archivo, "\n");

    // Escribir los datos de las filas en orden según el índice
    for (int fila = 0; fila < df->numFilas; fila++) {
        int filaReal = df->indice[fila]; // Orden según índice
        for (int col = 0; col < df->numColumnas; col++) {
            if (df->columnas[col].esNulo[filaReal]) {
                fprintf(archivo, "#N/A"); // Representación de valores nulos
            } else {
                if (df->columnas[col].tipo == NUMERICO) {
                    fprintf(archivo, "%d", ((int *)df->columnas[col].datos)[filaReal]);
                } else if (df->columnas[col].tipo == TEXTO || df->columnas[col].tipo == FECHA) {
                    fprintf(archivo, "%s", ((char **)df->columnas[col].datos)[filaReal]);
                }
            }
            if (col < df->numColumnas - 1) {
                fprintf(archivo, ",");
            }
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("\033[0;32mDataframe guardado exitosamente en '%s'.\033[0m\n", nombreFichero);
}

void filterDataframe(Dataframe *df, const char *comando) {
    if (!df) {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    char nombreColumna[30];
    char operador[4];
    char valorStr[100];
    int columnaIndex = -1;

    // Parsear el comando
    int numParametros = sscanf(comando, "%s %s %s", nombreColumna, operador, valorStr);
    if (numParametros != 3) {
        printf("\033[0;31mError: Número incorrecto de parámetros.\033[0m\n");
        return;
    }

    // Validar el operador
    if (strcmp(operador, "eq") != 0 && strcmp(operador, "neq") != 0 &&
        strcmp(operador, "gt") != 0 && strcmp(operador, "lt") != 0) {
        printf("\033[0;31mError: El operador debe ser 'eq', 'neq', 'gt' o 'lt'.\033[0m\n");
        return;
    }

    // Buscar el índice de la columna
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0) {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1) {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    Columna *columna = &df->columnas[columnaIndex];
    TipoDato tipoColumna = columna->tipo;

    // Mensaje de depuración
    printf("[DEBUG] Filtrando columna '%s' con operador '%s' y valor '%s'\n", nombreColumna, operador, valorStr);

    // Filtrar filas según el tipo de columna
    int nuevaNumFilas = 0;
    int *nuevoIndice = (int *)malloc(df->numFilas * sizeof(int)); // Nuevo índice para las filas filtradas

    for (int i = 0; i < df->numFilas; i++) {
        int filaReal = df->indice[i];
        int cumpleCondicion = 0;

        // Verificar valores nulos
        if (columna->esNulo[filaReal]) {
            printf("[DEBUG] Fila %d: Valor nulo, se descarta.\n", filaReal);
            continue;
        }

        if (tipoColumna == NUMERICO) {
            int valorColumna = ((int *)columna->datos)[filaReal];
            int valorFiltro = atoi(valorStr);
            printf("[DEBUG] Fila %d: Comparando %d con %d\n", filaReal, valorColumna, valorFiltro);

            if ((strcmp(operador, "eq") == 0 && valorColumna == valorFiltro) ||
                (strcmp(operador, "neq") == 0 && valorColumna != valorFiltro) ||
                (strcmp(operador, "gt") == 0 && valorColumna > valorFiltro) ||
                (strcmp(operador, "lt") == 0 && valorColumna < valorFiltro)) {
                cumpleCondicion = 1;
            }
        } else if (tipoColumna == TEXTO) {
            char *valorColumna = ((char **)columna->datos)[filaReal];
            printf("[DEBUG] Fila %d: Comparando '%s' con '%s'\n", filaReal, valorColumna, valorStr);

            if ((strcmp(operador, "eq") == 0 && strcmp(valorColumna, valorStr) == 0) ||
                (strcmp(operador, "neq") == 0 && strcmp(valorColumna, valorStr) != 0)) {
                cumpleCondicion = 1;
            }
        } else if (tipoColumna == FECHA) {
            struct tm fechaFiltro;
            if (!parseFecha(valorStr, &fechaFiltro)) {
                printf("\033[0;31mError: El valor '%s' no es una fecha válida.\033[0m\n", valorStr);
                free(nuevoIndice);
                return;
            }

            // Parsear fecha de la columna
            char *valorColumna = ((char **)columna->datos)[filaReal];
            struct tm fechaColumna;
            if (!parseFecha(valorColumna, &fechaColumna)) {
                printf("[DEBUG] Fila %d: Fecha inválida en la columna.\n", filaReal);
                continue; // Salir si la fecha en la columna es inválida
            }

            // Comparar fechas
            double diff = difftime(mktime(&fechaColumna), mktime(&fechaFiltro));

            if ((strcmp(operador, "eq") == 0 && diff == 0) ||
                (strcmp(operador, "neq") == 0 && diff != 0) ||
                (strcmp(operador, "gt") == 0 && diff > 0) ||
                (strcmp(operador, "lt") == 0 && diff < 0)) {
                cumpleCondicion = 1;
            }
        }

        if (cumpleCondicion) {
            printf("[DEBUG] Fila %d: Cumple la condición.\n", filaReal);
            nuevoIndice[nuevaNumFilas++] = filaReal; // Añadir al nuevo índice
        } else {
            printf("[DEBUG] Fila %d: No cumple la condición.\n", filaReal);
        }
    }

    // Actualizar el índice y el número de filas
    free(df->indice);
    df->indice = nuevoIndice;
    df->numFilas = nuevaNumFilas;

    printf("\033[0;32mEl dataframe ha sido filtrado por la columna '%s' con el operador '%s'.\033[0m\n", nombreColumna, operador);
}


//*****************************************************************
void delNull(Dataframe *df, const char *nombreColumna) {
    if (!df) {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    int columnaIndex = -1;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0) {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1) {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    Columna *columna = &df->columnas[columnaIndex];
    int filasEliminadas = 0;
    int *nuevoIndice = (int *)malloc(df->numFilas * sizeof(int)); // Nuevo índice para las filas válidas
    int nuevaNumFilas = 0;

    // Recorremos las filas del dataframe
    for (int i = 0; i < df->numFilas; i++) {
        int filaReal = df->indice[i];
        if (columna->esNulo[filaReal]) {
            // Si la fila tiene valor nulo en la columna indicada, se elimina
            filasEliminadas++;
        } else {
            // Si no es nulo, la mantenemos en el nuevo índice
            nuevoIndice[nuevaNumFilas++] = filaReal;
        }
    }

    if (filasEliminadas > 0) {
        // Actualizar el dataframe
        free(df->indice);
        df->indice = nuevoIndice;
        df->numFilas = nuevaNumFilas;
        printf("\033[0;32mSe han eliminado %d filas con valores nulos en la columna '%s'.\033[0m\n", filasEliminadas, nombreColumna);
    } else {
        free(nuevoIndice);
        printf("\033[0;31mNo se encontraron valores nulos en la columna '%s'.\033[0m\n", nombreColumna);
    }
}


void delColumn(Dataframe *df, const char *nombreColumna) {
    if (!df) {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    int columnaIndex = -1;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0) {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1) {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    // Liberar la memoria de la columna
    Columna *columna = &df->columnas[columnaIndex];
    free(columna->nombre);
    free(columna->esNulo);
    free(columna->datos);

    // Desplazar todas las columnas posteriores a la izquierda
    for (int i = columnaIndex; i < df->numColumnas - 1; i++) {
        df->columnas[i] = df->columnas[i + 1];
    }

    // Reducir el número de columnas
    df->numColumnas--;

    printf("\033[0;32mLa columna '%s' ha sido eliminada.\033[0m\n", nombreColumna);
}






//****************************************************************+
// void quarterColumn(Dataframe *df, const char *nombreColumna, const char *nombreNuevaColumna) {
//     // Implementación del comando quarter, crea una nueva columna basada en el trimestre de la fecha.
// }
