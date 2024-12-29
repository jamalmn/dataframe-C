#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "lib.h"

// *******************************************************
// *************** VARIABLES GLOBALES *******************
// *******************************************************
Dataframe *dataframeActivo = NULL; // Puntero al dataframe activo
int indiceDataframe = -1;          // Índice del dataframe activo
Lista listaDataframes = {0, NULL}; // Lista de dataframes en memoria

// *******************************************************
// *********** FUNCIONES DE INICIALIZACIÓN **************
// *******************************************************
void mostrarPromptDataframe()
{
    if (dataframeActivo != NULL)
    {
        printf("[%s: %d filas, %d columnas]:> ", dataframeActivo->nombre, dataframeActivo->numFilas, dataframeActivo->numColumnas);
    }
    else
    {
        printf("[?]:> ");
    }
    fflush(stdout);
}

void inicializarPrograma()
{
    // Título del programa en azul y negrita
    printf("\033[1;34m=== \033[1;37mPROGRAMA DE MANEJO DE DATAFRAMES \033[1;34m===\033[0m\n\n");

    // Encabezado con texto verde
    printf("\033[1;32mDATOS DEL ALUMNO:\033[0m\n");

    // Detalles con un fondo oscuro y texto en blanco
    printf("\033[1;37m\033[48;5;235mNOMBRE + APELLIDOS: \033[1;33mJamal Menchi Hajji\033[0m\n");
    printf("\033[1;37m\033[48;5;235mCORREO: \033[1;36mjamal.menchi@goumh.umh.es\033[0m\n\n");

    // Mostrar estado de los ejercicios
    printf("\033[1;32mESTADO DE LOS EJERCICIOS:\033[0m\n");
    printf("\033[1;37mEjercicio 1: \033[1;32mHECHO\033[0m\n");
    printf("\033[1;37mEjercicio 2: \033[1;32mHECHO\033[0m\n");
    printf("\033[1;37mEjercicio 3: \033[1;32mHECHO\033[0m\n");
    printf("\033[1;37mEjercicio 4: \033[1;32mHECHO\033[0m\n\n");
}

// *******************************************************
// ************** FUNCIONES DE COMANDOS *****************
// *******************************************************
void manejarComandoLoad(const char *argumento)
{
    char nombreFichero[100];
    if (sscanf(argumento, "%s", nombreFichero) == 1)
    {
        dataframeActivo = cargarCSV(nombreFichero);
        if (dataframeActivo)
        {
            // Asignar un nombre único por defecto
            snprintf(dataframeActivo->nombre, sizeof(dataframeActivo->nombre), "df%d", listaDataframes.numDFs);
            printf("\033[0;32mArchivo %s cargado correctamente como '%s'.\033[0m\n", nombreFichero, dataframeActivo->nombre);

            // Añadir a la lista de dataframes
            Nodo *nuevoNodo = (Nodo *)malloc(sizeof(Nodo));
            nuevoNodo->df = dataframeActivo;
            nuevoNodo->siguiente = listaDataframes.primero;
            listaDataframes.primero = nuevoNodo;
            listaDataframes.numDFs++;
        }
        else
        {
            printf("\033[0;31mError al cargar el archivo %s.\033[0m\n", nombreFichero);
        }
    }
    else
    {
        printf("\033[0;31mDebe especificar un nombre de fichero.\033[0m\n");
    }
}

void manejarComandoView(const char *argumento)
{
    int numFilas = 10;
    if (dataframeActivo != NULL)
    {
        if (sscanf(argumento, "%d", &numFilas) == 1)
        {
            if (numFilas < 0)
            {
                viewDataframe(dataframeActivo, numFilas); // Manejo de negativos
            }
            else if (numFilas > 0)
            {
                viewDataframe(dataframeActivo, numFilas);
            }
            else
            {
                printf("\033[0;31mEl número de filas debe ser distinto de 0.\033[0m\n");
            }
        }
        else
        {
            viewDataframe(dataframeActivo, 10); // Valor predeterminado
        }
    }
    else
    {
        printf("\033[0;31mNo hay un dataframe activo para mostrar.\033[0m\n");
    }
}

void manejarComandoMeta()
{
    if (dataframeActivo)
    {
        mostrarMetadatos(dataframeActivo);
    }
    else
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
    }
}

void manejarComandoSort(const char *comando)
{
    char nombreColumna[100];
    char orden[4] = "asc"; // Por defecto, ascendente
    int numParametros = sscanf(comando, "%s %s", nombreColumna, orden);

    if (numParametros < 1 || numParametros > 2)
    {
        printf("\033[0;31mError: número incorrecto de parámetros.\033[0m\n");
        return;
    }

    // Validar que la columna existe y ejecutar la función
    sortDataframe(dataframeActivo, comando);
}

void manejarComandoSave(const char *comando)
{
    char nombreFichero[100];
    if (sscanf(comando, "%s", nombreFichero) == 1)
    {
        saveDataframe(dataframeActivo, nombreFichero);
    }
    else
    {
        printf("\033[0;31mError: debe especificar el nombre del archivo.\033[0m\n");
    }
}

void manejarComandoFilter(const char *comando)
{
    // Lógica para procesar el comando filter, como obtener la columna, operador y valor
    filterDataframe(dataframeActivo, comando);
}

void manejarComandoDelNull(const char *comando)
{
    // Obtener el nombre de la columna y eliminar las filas con valores nulos
    delNull(dataframeActivo, comando);
}

void manejarComandoDelColum(const char *comando)
{
    // Obtener el nombre de la columna y eliminarla del dataframe
    delColumn(dataframeActivo, comando);
}

void manejarComandoQuarter(const char *comando)
{
    char nombreColumna[100];
    char nombreNuevaColumna[100];

    if (sscanf(comando, "%s %s", nombreColumna, nombreNuevaColumna) == 2)
    {
        quarterColumn(dataframeActivo, nombreColumna, nombreNuevaColumna);
    }
    else
    {
        printf("\033[0;31mError: parámetros incorrectos para quarter.\033[0m\n");
    }
}

void manejarComandoName(const char *argumento)
{
    if (!dataframeActivo)
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    char nuevoNombre[51];
    if (sscanf(argumento, "%50s", nuevoNombre) != 1)
    {
        printf("\033[0;31mDebe proporcionar un nombre válido.\033[0m\n");
        return;
    }

    // Verificar si el nombre ya existe
    Nodo *actual = listaDataframes.primero;
    while (actual)
    {
        if (strcmp(actual->df->nombre, nuevoNombre) == 0)
        {
            printf("\033[0;31mError: Ya existe un dataframe con el nombre '%s'.\033[0m\n", nuevoNombre);
            return;
        }
        actual = actual->siguiente;
    }

    // Asignar el nuevo nombre
    strncpy(dataframeActivo->nombre, nuevoNombre, sizeof(dataframeActivo->nombre) - 1);
    dataframeActivo->nombre[sizeof(dataframeActivo->nombre) - 1] = '\0';
    printf("\033[0;32mEl nombre del dataframe ha sido cambiado a '%s'.\033[0m\n", dataframeActivo->nombre);
}

void manejarComandoList()
{
    if (listaDataframes.numDFs == 0)
    {
        printf("\033[0;31mNo hay dataframes cargados en memoria.\033[0m\n");
        return;
    }

    printf("\033[1;34mListado de dataframes cargados:\033[0m\n");
    Nodo *actual = listaDataframes.primero;

    while (actual)
    {
        Dataframe *df = actual->df;
        printf("\033[0;32m%s: %d filas, %d columnas\033[0m\n", df->nombre, df->numFilas, df->numColumnas);
        actual = actual->siguiente;
    }
}

void manejarComandoDesconocido()
{
    printf("\033[0;31mComando no válido.\033[0m\n");
}

void manejarComandoPrefix(const char *argumento)
{
    if (!dataframeActivo)
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    char nombreColumna[30], nuevaColumna[30];
    int n;

    // Parsear los argumentos
    if (sscanf(argumento, "%s %d %s", nombreColumna, &n, nuevaColumna) != 3)
    {
        printf("\033[0;31mError: Parámetros incorrectos. Uso: prefix <columna> n <nueva_columna>.\033[0m\n");
        return;
    }

    if (n <= 0)
    {
        printf("\033[0;31mError: 'n' debe ser mayor que 0.\033[0m\n");
        return;
    }

    // Verificar que la columna exista y sea de tipo TEXTO
    int columnaIndex = -1;
    for (int i = 0; i < dataframeActivo->numColumnas; i++)
    {
        if (strcmp(dataframeActivo->columnas[i].nombre, nombreColumna) == 0)
        {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1)
    {
        printf("\033[0;31mError: La columna '%s' no existe.\033[0m\n", nombreColumna);
        return;
    }

    if (dataframeActivo->columnas[columnaIndex].tipo != TEXTO)
    {
        printf("\033[0;31mError: La columna '%s' no es de tipo TEXTO.\033[0m\n", nombreColumna);
        return;
    }

    // Verificar que el nombre de la nueva columna no exista
    for (int i = 0; i < dataframeActivo->numColumnas; i++)
    {
        if (strcmp(dataframeActivo->columnas[i].nombre, nuevaColumna) == 0)
        {
            printf("\033[0;31mError: Ya existe una columna con el nombre '%s'.\033[0m\n", nuevaColumna);
            return;
        }
    }

    // Crear espacio para la nueva columna
    Columna *nuevasColumnas = realloc(dataframeActivo->columnas, (dataframeActivo->numColumnas + 1) * sizeof(Columna));
    if (!nuevasColumnas)
    {
        printf("\033[0;31mError: No se pudo asignar memoria para la nueva columna.\033[0m\n");
        return;
    }

    dataframeActivo->columnas = nuevasColumnas;
    Columna *nuevaColumnaPtr = &dataframeActivo->columnas[dataframeActivo->numColumnas];
    dataframeActivo->numColumnas++;

    // Inicializar la nueva columna
    strncpy(nuevaColumnaPtr->nombre, nuevaColumna, sizeof(nuevaColumnaPtr->nombre) - 1);
    nuevaColumnaPtr->nombre[sizeof(nuevaColumnaPtr->nombre) - 1] = '\0';
    nuevaColumnaPtr->tipo = TEXTO;
    nuevaColumnaPtr->datos = calloc(dataframeActivo->numFilas, sizeof(char *));
    nuevaColumnaPtr->esNulo = calloc(dataframeActivo->numFilas, sizeof(unsigned char));

    if (!nuevaColumnaPtr->datos || !nuevaColumnaPtr->esNulo)
    {
        printf("\033[0;31mError: No se pudo asignar memoria para los datos de la nueva columna.\033[0m\n");
        free(nuevaColumnaPtr->datos);
        free(nuevaColumnaPtr->esNulo);
        return;
    }

    // Llenar la nueva columna con los prefijos
    Columna *columnaOriginal = &dataframeActivo->columnas[columnaIndex];
    for (int i = 0; i < dataframeActivo->numFilas; i++)
    {
        int filaReal = dataframeActivo->indice[i];

        if (columnaOriginal->esNulo[filaReal])
        {
            nuevaColumnaPtr->esNulo[filaReal] = 1;
            ((char **)nuevaColumnaPtr->datos)[filaReal] = strdup("#N/A");
        }
        else
        {
            char *valorOriginal = ((char **)columnaOriginal->datos)[filaReal];
            int len = strlen(valorOriginal);
            char *prefijo = strndup(valorOriginal, (n < len) ? n : len);
            ((char **)nuevaColumnaPtr->datos)[filaReal] = prefijo;
            nuevaColumnaPtr->esNulo[filaReal] = 0;
        }
    }

    printf("\033[0;32mLa nueva columna '%s' ha sido creada correctamente.\033[0m\n", nuevaColumna);
}

// *******************************************************
// ************** FUNCIONES AUXILIARES ******************
// *******************************************************
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

void liberarMemoria()
{
    liberarMemoriaLista();
    printf("memoria liberada correctamente\n");
}

int esNumerico(const char *str)
{
    int i = 0;
    int tienePunto = 0; // Indica si ya se encontró un punto decimal

    // Manejar el signo al principio
    if (str[i] == '-' || str[i] == '+')
    {
        i++;
    }

    int tieneDigito = 0; // Asegura que haya al menos un dígito

    for (; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
        {
            tieneDigito = 1; // Encontró al menos un dígito
        }
        else if (str[i] == '.')
        {
            if (tienePunto)
            {
                return 0; // Más de un punto decimal
            }
            tienePunto = 1; // Marcar que hay un punto decimal
        }
        else
        {
            return 0; // Caracter no permitido
        }
    }

    // Retornar válido solo si tiene al menos un dígito
    return tieneDigito;
}

int esBisiesto(int anio)
{
    return (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0));
}

int esFecha(char *str)
{
    // Verificar longitud y formato básico
    if (strlen(str) == 11)
    {
        if (str[10] != '\n')
        {
            return 0;
        }
    }
    else if (!strlen(str) == 10)
    {
        return 0;
    }

    // Detectar formato con barras (YYYY/MM/DD)
    if (str[4] == '/' && str[7] == '/')
    {
        str[4] = '-';
        str[7] = '-';
    }

    // Verificar formato básico YYYY-MM-DD
    for (int i = 0; i < 10; i++)
    {
        if ((i == 4 || i == 7))
        {
            if (str[i] != '-')
            {
                return 0; // Falta el guion
            }
        }
        else if (!isdigit(str[i]))
        {
            return 0; // No es un dígito donde debería serlo
        }
    }

    // Verificar valores numéricos
    int year = atoi(str);
    int month = atoi(str + 5);
    int day = atoi(str + 8);

    if (month < 1 || month > 12 || day < 1 || day > 31)
    {
        return 0; // Mes o día fuera de rango
    }

    // Verificar días máximos por mes
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        return 0; // Meses de 30 días
    }
    if (month == 2)
    { // Febrero
        int isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if ((isLeapYear && day > 29) || (!isLeapYear && day > 28))
        {
            return 0; // Días de febrero
        }
    }

    return 1; // Fecha válida
}

int parseFecha(const char *str, struct tm *fecha)
{
    // Usamos sscanf para descomponer la fecha en año, mes y día
    int anio, mes, dia;
    if (sscanf(str, "%4d-%2d-%2d", &anio, &mes, &dia) != 3)
    {
        return 0; // Error de parseo
    }

    // Asignamos los valores a la estructura tm
    fecha->tm_year = anio - 1900; // Años desde 1900
    fecha->tm_mon = mes - 1;      // Meses de 0 a 11
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
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isalnum(str[i]) && !isspace(str[i]) && str[i] != '_')
        {
            return 0; // No es texto si contiene caracteres no permitidos
        }
    }

    return 1; // es texto valido
}

char *strndup(const char *str, size_t n) {
    size_t len = strlen(str);
    if (n < len) {
        len = n;
    }

    char *result = (char *)malloc(len + 1); // +1 para el terminador null
    if (!result) {
        return NULL;
    }

    strncpy(result, str, len);
    result[len] = '\0'; // Asegurarse de que está terminado con null
    return result;
}

int contarComas(const char *buffer) {
    int contador = 0;

    // Recorrer la cadena caracter por caracter
    while (*buffer != '\0') {
        if (*buffer == ',') {
            contador++;
        }
        buffer++;
    }

    return contador;
}
// *******************************************************
// *********** FUNCIONES DE MANEJO DE CSV ***************
// *******************************************************
Dataframe *cargarCSV(char *nombreFichero)
{
    FILE *archivo = fopen(nombreFichero, "r");
    if (!archivo)
    {
        printf("\033[0;31mError: no se pudo abrir el archivo %s.\033[0m\n", nombreFichero);
        return NULL;
    }

    Dataframe *df = (Dataframe *)malloc(sizeof(Dataframe));
    if (!df)
    {
        printf("\033[0;31mError: no se pudo asignar memoria para el dataframe.\033[0m\n");
        fclose(archivo);
        return NULL;
    }

    memset(df, 0, sizeof(Dataframe));

    // Contar columnas
    df->numColumnas = contarColumnas(archivo);
    if (df->numColumnas <= 0)
    {
        printf("\033[0;31mError: el archivo no contiene columnas válidas.\033[0m\n");
        free(df);
        fclose(archivo);
        return NULL;
    }

    // Inicializar columnas
    inicializarDataframe(df, df->numColumnas);

    // Contar Filas
    df->numFilas = contarFilas(archivo) - 1;
    fseek(archivo, 0, SEEK_SET); // Reset pointer to start

    // Leer encabezados
    if(leerEncabezados(archivo, df)==0){
        printf("\033[0;31mError: En el encabezado del archivo no puede haber nulos.\033[0m\n");
        free(df);
        fclose(archivo);
        return NULL;
    }

    // Determinar tipos de datos
    if(determinarTipos(archivo, df)==0){
        printf("\033[0;31mError: La primera linea de datos (segunda del fichero) no debe contener nulos.\033[0m\n");
        free(df);
        fclose(archivo);
        return NULL;
    }

    // Debug: Confirm pointer after skipping headers
    char debugBuffer[1024];
    fseek(archivo, 0, SEEK_SET);                      // Reset pointer to start
    fgets(debugBuffer, sizeof(debugBuffer), archivo); // Skip header row
    fgets(debugBuffer, sizeof(debugBuffer), archivo); // Skip type row

    fseek(archivo, 0, SEEK_SET);                      // Reset pointer to start
    fgets(debugBuffer, sizeof(debugBuffer), archivo); // Skip header row
    leerDatos(archivo, df);

    fclose(archivo);
    return df;
}

int contarFilas(FILE *archivo)
{

    char buffer[1024];
    int cont = 0;
    while (fgets(buffer, sizeof(buffer), archivo))
    {
        cont++;
    }

    return cont;
}

static int contarColumnas(FILE *archivo)
{
    char buffer[1024];
    int numColumnas = 0;

    // Leer la primera línea
    if (fgets(buffer, sizeof(buffer), archivo))
    {
        char *token = strtok(buffer, ",");
        while (token)
        {
            numColumnas++;
            token = strtok(NULL, ",");
        }
    }
    rewind(archivo);
    return numColumnas;
}

static void inicializarDataframe(Dataframe *df, int numColumnas)
{
    df->columnas = (Columna *)malloc(numColumnas * sizeof(Columna));
    if (!df->columnas)
    {
        printf("\033[0;31mError: no se pudo asignar memoria para las columnas.\033[0m\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numColumnas; i++)
    {
        memset(&df->columnas[i], 0, sizeof(Columna));
    }
}

static int  leerEncabezados(FILE *archivo, Dataframe *df)
{
    char buffer[1024];

    if (fgets(buffer, sizeof(buffer), archivo))
    {
        char *token = strtok(buffer, ",");
        int i = 0;
        while (token && i < df->numColumnas)
        {
            token[strcspn(token, "\r\n")] = '\0'; // Limpiar saltos de línea

            // Asignar nombre predeterminado si está vacío
            if (strlen(token) == 0)
            {
                return 0;
            }
            else
            {
                strncpy(df->columnas[i].nombre, token, sizeof(df->columnas[i].nombre) - 1);
            }
            df->columnas[i].nombre[sizeof(df->columnas[i].nombre) - 1] = '\0'; 
            token = strtok(NULL, ",");
            i++;
        }
    }

    return 1;
}

static int determinarTipos(FILE *archivo, Dataframe *df)
{
    char buffer[1024];

    if (fgets(buffer, sizeof(buffer), archivo))
    {
        if(contarComas(buffer) != df->numColumnas-1){
            return 0;
        }
        char *token = strtok(buffer, ",");
        int i = 0;
        
        while (token && i < df->numColumnas)
        {
            token[strcspn(token, " \t\r\n")] = '\0';


            if(token[0] == '\0'){
                return 0;
            }else if (esNumerico(token))
            {
                df->columnas[i].tipo = NUMERICO;
                df->columnas[i].datos = malloc(df->numFilas * sizeof(float));
            }
            else if (esFecha(token))
            {
                df->columnas[i].tipo = FECHA;
                df->columnas[i].datos = malloc(df->numFilas * sizeof(char *));
            }
            else
            {
                df->columnas[i].tipo = TEXTO;
                df->columnas[i].datos = malloc(df->numFilas * sizeof(char *));
            }
            df->columnas[i].esNulo = malloc(df->numFilas * sizeof(unsigned char));
            token = strtok(NULL, ",");
            i++;
        }
    }
    else
    {
        // Si no hay segunda fila, inicializar todo como TEXTO por defecto
        for (int i = 0; i < df->numColumnas; i++)
        {
            df->columnas[i].tipo = TEXTO;
            df->columnas[i].datos = malloc(df->numFilas * sizeof(char *));
            df->columnas[i].esNulo = malloc(df->numFilas * sizeof(unsigned char));
        }
    }
    return 1;
}

static void leerDatos(FILE *archivo, Dataframe *df)
{
    char buffer[1024];
    int numFilas = 0;

    // Leer las filas del archivo una por una
    while (fgets(buffer, sizeof(buffer), archivo))
    {
        char *inicio = buffer;
        int i = 0;

        while (i < df->numColumnas)
        {
            char *fin = strchr(inicio, ','); // Buscar la siguiente coma

            // Manejar el caso de la última columna
            if (fin == NULL)
            {
                fin = inicio + strlen(inicio); // Tomar hasta el final de la línea
            }
            else
            {
                *fin = '\0'; // Terminar el token actual
            }

            // Remover espacios en blanco y saltos de línea al final
            inicio[strcspn(inicio, " \t\r\n")] = '\0';

            if (inicio[0] == '\0') // Detectar valores nulos
            {
                df->columnas[i].esNulo[numFilas] = 1;
            }
            else
            {
                df->columnas[i].esNulo[numFilas] = 0;

                if (df->columnas[i].tipo == NUMERICO)
                {
                    if (esNumerico(inicio))
                    {
                        ((float *)df->columnas[i].datos)[numFilas] = atof(inicio);
                    }
                    else
                    {
                        df->columnas[i].esNulo[numFilas] = 1;
                    }
                }
                else if (df->columnas[i].tipo == FECHA)
                {
                    if (esFecha(inicio))
                    {
                        ((char **)df->columnas[i].datos)[numFilas] = strdup(inicio);
                    }
                    else
                    {
                        df->columnas[i].esNulo[numFilas] = 1;
                    }
                }
                else
                {
                    ((char **)df->columnas[i].datos)[numFilas] = strdup(inicio);
                }
            }

            // Avanzar al siguiente valor
            inicio = fin + 1; 
            i++;
        }
        numFilas++;
    }

    df->indice = malloc(numFilas * sizeof(int));
    for (int i = 0; i < numFilas; i++)
    {
        df->indice[i] = i;
    }
}

void viewDataframe(Dataframe *df, int n)
{
    if (!df)
    {
        printf("\033[0;31mError: no hay dataframe activo.\033[0m\n");
        return;
    }

    int start = 0, end = n, step = 1;

    if (n < 0)
    {
        n = -n;
        if (n > df->numFilas)
            n = df->numFilas;

        start = df->numFilas - 1;
        end = df->numFilas - n - 1;
        step = -1;
    }
    else if (n > df->numFilas)
    {
        n = df->numFilas;
    }

    // Crear una línea divisoria para la tabla
    printf("+");
    for (int i = 0; i < df->numColumnas; i++)
    {
        for (int j = 0; j < 17; j++) // Ancho fijo de 15 para las columnas
            printf("-");
        printf("+");
    }
    printf("\n");

    // Mostrar los nombres de las columnas
    printf("|");
    for (int i = 0; i < df->numColumnas; i++)
    {
        printf("\033[0;32m %-16s\033[0m|", df->columnas[i].nombre); // Alinear nombres
    }
    printf("\n");

    // Crear otra línea divisoria debajo de las cabeceras
    printf("+");
    for (int i = 0; i < df->numColumnas; i++)
    {
        for (int j = 0; j < 17; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // Mostrar filas respetando el índice
    for (int i = start; i != end; i += step)
    {
        int filaReal = df->indice[i]; // Usar el índice para acceder a las filas reales
        printf("|");                  // Comienzo de la fila
        for (int col = 0; col < df->numColumnas; col++)
        {
            if (df->columnas[col].esNulo[filaReal])
            {
                printf("\033[0;32m %-16s\033[0m|", "#N/A");
            }
            else
            {
                if (df->columnas[col].tipo == NUMERICO)
                {
                    printf("\033[0;32m %-16.2f\033[0m|", ((float *)df->columnas[col].datos)[filaReal]); // Mostrar enteros
                }
                else if (df->columnas[col].tipo == TEXTO || df->columnas[col].tipo == FECHA)
                {
                    printf("\033[0;32m %-16s\033[0m|", ((char **)df->columnas[col].datos)[filaReal]); // Mostrar texto o fecha
                }
            }
        }
        printf("\n");
    }

    // Crear una línea final divisoria
    printf("+");
    for (int i = 0; i < df->numColumnas; i++)
    {
        for (int j = 0; j < 17; j++)
            printf("-");
        printf("+");
    }
    printf("\n");
}

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

void sortDataframe(Dataframe *df, const char *comando)
{
    if (!df)
    {
        printf("\033[0;31mError: no hay un dataframe activo.\033[0m\n");
        return;
    }

    // Validar el índice
    if (!df->indice)
    {
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

    if (numParametros < 1 || numParametros > 2)
    {
        printf("\033[0;31mError: número incorrecto de parámetros.\033[0m\n");
        return;
    }

    // Validar el orden
    if (numParametros == 2 && strcmp(orden, "asc") != 0 && strcmp(orden, "des") != 0)
    {
        printf("\033[0;31mError: El parámetro de orden debe ser 'asc' o 'des'.\033[0m\n");
        return;
    }

    // Buscar el índice de la columna
    for (int i = 0; i < df->numColumnas; i++)
    {
        // printf("[DEBUG] Comparando columna '%s' con '%s'\n", df->columnas[i].nombre, nombreColumna);
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0)
        {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1)
    {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    // printf("[DEBUG] Columna encontrada en el índice %d\n", columnaIndex);

    // Mostrar índice inicial
    // printf("[DEBUG] Índice inicial: ");
    for (int i = 0; i < df->numFilas; i++)
    {
        printf("%d ", df->indice[i]);
    }
    printf("\n");

    // Ordenar las filas utilizando el array 'indice'
    for (int i = 0; i < df->numFilas - 1; i++)
    {
        for (int j = i + 1; j < df->numFilas; j++)
        {
            int filaI = df->indice[i];
            int filaJ = df->indice[j];
            int comparar = 0;

            // Comparar valores nulos
            if (df->columnas[columnaIndex].esNulo[filaI] && df->columnas[columnaIndex].esNulo[filaJ])
            {
                comparar = 0; // Ambos nulos
                // printf("[DEBUG] Ambos valores son nulos\n");
            }
            else if (df->columnas[columnaIndex].esNulo[filaI])
            {
                comparar = -1; // FilaI nulo, va primero
                // printf("[DEBUG] Fila %d (índice %d) es nulo\n", i, filaI);
            }
            else if (df->columnas[columnaIndex].esNulo[filaJ])
            {
                comparar = 1; // FilaJ nulo, va primero
                // printf("[DEBUG] Fila %d (índice %d) es nulo\n", j, filaJ);
            }
            else
            {
                // Comparar según el tipo de dato
                if (df->columnas[columnaIndex].tipo == NUMERICO)
                {
                    int valorI = ((int *)df->columnas[columnaIndex].datos)[filaI];
                    int valorJ = ((int *)df->columnas[columnaIndex].datos)[filaJ];
                    comparar = (valorI > valorJ) - (valorI < valorJ);
                    // printf("[DEBUG] Comparando NUMERICO: %d y %d\n", valorI, valorJ);
                }
                else if (df->columnas[columnaIndex].tipo == TEXTO)
                {
                    char *valorI = ((char **)df->columnas[columnaIndex].datos)[filaI];
                    char *valorJ = ((char **)df->columnas[columnaIndex].datos)[filaJ];
                    comparar = strcmp(valorI, valorJ);
                    // printf("[DEBUG] Comparando TEXTO: '%s' y '%s'\n", valorI, valorJ);
                }
            }

            // Realizar intercambio si es necesario
            if ((strcmp(orden, "asc") == 0 && comparar > 0) || (strcmp(orden, "des") == 0 && comparar < 0))
            {
                int temp = df->indice[i];
                df->indice[i] = df->indice[j];
                df->indice[j] = temp;
                // printf("[DEBUG] Intercambiando índices: %d y %d\n", i, j);
            }
        }
    }

    // Mostrar índice final
    // printf("[DEBUG] Índice final: ");
    for (int i = 0; i < df->numFilas; i++)
    {
        printf("%d ", df->indice[i]);
    }
    printf("\n");

    printf("\033[0;32mEl dataframe ha sido ordenado por la columna '%s' en orden %s.\033[0m\n", nombreColumna, orden);
}

void saveDataframe(Dataframe *df, const char *nombreFichero)
{
    if (!df)
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    FILE *archivo = fopen(nombreFichero, "w");
    if (!archivo)
    {
        printf("\033[0;31mError: No se pudo crear el archivo '%s'.\033[0m\n", nombreFichero);
        return;
    }

    // Escribir los nombres de las columnas
    for (int i = 0; i < df->numColumnas; i++)
    {
        fprintf(archivo, "%s", df->columnas[i].nombre);
        if (i < df->numColumnas - 1)
        {
            fprintf(archivo, ",");
        }
    }
    fprintf(archivo, "\n");

    // Escribir los datos de las filas en orden según el índice
    for (int fila = 0; fila < df->numFilas; fila++)
    {
        int filaReal = df->indice[fila]; // Orden según índice
        for (int col = 0; col < df->numColumnas; col++)
        {
            if (df->columnas[col].esNulo[filaReal])
            {
                fprintf(archivo, "#N/A"); // Representación de valores nulos
            }
            else
            {
                if (df->columnas[col].tipo == NUMERICO)
                {
                    fprintf(archivo, "%.2f", ((float *)df->columnas[col].datos)[filaReal]);
                }
                else if (df->columnas[col].tipo == TEXTO || df->columnas[col].tipo == FECHA)
                {
                    fprintf(archivo, "%s", ((char **)df->columnas[col].datos)[filaReal]);
                }
            }
            if (col < df->numColumnas - 1)
            {
                fprintf(archivo, ",");
            }
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("\033[0;32mDataframe guardado exitosamente en '%s'.\033[0m\n", nombreFichero);
}

void filterDataframe(Dataframe *df, const char *comando)
{
    if (!df)
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    char nombreColumna[30];
    char operador[4];
    char valorStr[100];
    int columnaIndex = -1;

    // Parsear el comando
    int numParametros = sscanf(comando, "%s %s %s", nombreColumna, operador, valorStr);
    if (numParametros != 3)
    {
        printf("\033[0;31mError: Número incorrecto de parámetros.\033[0m\n");
        return;
    }

    // Validar el operador
    if (strcmp(operador, "eq") != 0 && strcmp(operador, "neq") != 0 &&
        strcmp(operador, "gt") != 0 && strcmp(operador, "lt") != 0)
    {
        printf("\033[0;31mError: El operador debe ser 'eq', 'neq', 'gt' o 'lt'.\033[0m\n");
        return;
    }

    // Buscar el índice de la columna
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0)
        {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1)
    {
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

    for (int i = 0; i < df->numFilas; i++)
    {
        int filaReal = df->indice[i];
        int cumpleCondicion = 0;

        // Verificar valores nulos
        if (columna->esNulo[filaReal])
        {
            printf("[DEBUG] Fila %d: Valor nulo, se descarta.\n", filaReal);
            continue;
        }

        if (tipoColumna == NUMERICO)
        {
            int valorColumna = ((int *)columna->datos)[filaReal];
            int valorFiltro = atoi(valorStr);
            printf("[DEBUG] Fila %d: Comparando %d con %d\n", filaReal, valorColumna, valorFiltro);

            if ((strcmp(operador, "eq") == 0 && valorColumna == valorFiltro) ||
                (strcmp(operador, "neq") == 0 && valorColumna != valorFiltro) ||
                (strcmp(operador, "gt") == 0 && valorColumna > valorFiltro) ||
                (strcmp(operador, "lt") == 0 && valorColumna < valorFiltro))
            {
                cumpleCondicion = 1;
            }
        }
        else if (tipoColumna == TEXTO)
        {
            char *valorColumna = ((char **)columna->datos)[filaReal];
            printf("[DEBUG] Fila %d: Comparando '%s' con '%s'\n", filaReal, valorColumna, valorStr);

            if ((strcmp(operador, "eq") == 0 && strcmp(valorColumna, valorStr) == 0) ||
                (strcmp(operador, "neq") == 0 && strcmp(valorColumna, valorStr) != 0))
            {
                cumpleCondicion = 1;
            }
        }
        else if (tipoColumna == FECHA)
        {
            struct tm fechaFiltro;
            if (!parseFecha(valorStr, &fechaFiltro))
            {
                printf("\033[0;31mError: El valor '%s' no es una fecha válida.\033[0m\n", valorStr);
                free(nuevoIndice);
                return;
            }

            // Parsear fecha de la columna
            char *valorColumna = ((char **)columna->datos)[filaReal];
            struct tm fechaColumna;
            if (!parseFecha(valorColumna, &fechaColumna))
            {
                printf("[DEBUG] Fila %d: Fecha inválida en la columna.\n", filaReal);
                continue; // Salir si la fecha en la columna es inválida
            }

            // Comparar fechas
            double diff = difftime(mktime(&fechaColumna), mktime(&fechaFiltro));

            if ((strcmp(operador, "eq") == 0 && diff == 0) ||
                (strcmp(operador, "neq") == 0 && diff != 0) ||
                (strcmp(operador, "gt") == 0 && diff > 0) ||
                (strcmp(operador, "lt") == 0 && diff < 0))
            {
                cumpleCondicion = 1;
            }
        }

        if (cumpleCondicion)
        {
            printf("[DEBUG] Fila %d: Cumple la condición.\n", filaReal);
            nuevoIndice[nuevaNumFilas++] = filaReal; // Añadir al nuevo índice
        }
        else
        {
            printf("[DEBUG] Fila %d: No cumple la condición.\n", filaReal);
        }
    }

    // Actualizar el índice y el número de filas
    free(df->indice);
    df->indice = nuevoIndice;
    df->numFilas = nuevaNumFilas;

    printf("\033[0;32mEl dataframe ha sido filtrado por la columna '%s' con el operador '%s'.\033[0m\n", nombreColumna, operador);
}

void delNull(Dataframe *df, const char *nombreColumna)
{
    if (!df)
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    int columnaIndex = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0)
        {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1)
    {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    Columna *columna = &df->columnas[columnaIndex];
    int filasEliminadas = 0;
    int *nuevoIndice = (int *)malloc(df->numFilas * sizeof(int)); // Nuevo índice para las filas válidas
    int nuevaNumFilas = 0;

    // Recorremos las filas del dataframe
    for (int i = 0; i < df->numFilas; i++)
    {
        int filaReal = df->indice[i];
        if (columna->esNulo[filaReal])
        {
            // Si la fila tiene valor nulo en la columna indicada, se elimina
            filasEliminadas++;
        }
        else
        {
            // Si no es nulo, la mantenemos en el nuevo índice
            nuevoIndice[nuevaNumFilas++] = filaReal;
        }
    }

    if (filasEliminadas > 0)
    {
        // Actualizar el dataframe
        free(df->indice);
        df->indice = nuevoIndice;
        df->numFilas = nuevaNumFilas;
        printf("\033[0;32mSe han eliminado %d filas con valores nulos en la columna '%s'.\033[0m\n", filasEliminadas, nombreColumna);
    }
    else
    {
        free(nuevoIndice);
        printf("\033[0;31mNo se encontraron valores nulos en la columna '%s'.\033[0m\n", nombreColumna);
    }
}

void delColumn(Dataframe *df, const char *nombreColumna)
{
    if (!df)
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    // Buscar la columna por nombre
    int columnaIndex = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0)
        {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1)
    {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    // Mensaje de depuración
    printf("[DEBUG] Eliminando columna '%s'.\n", df->columnas[columnaIndex].nombre);

    // Liberar la memoria asociada a la columna
    if (df->columnas[columnaIndex].esNulo)
    {
        free(df->columnas[columnaIndex].esNulo);
        df->columnas[columnaIndex].esNulo = NULL;
    }
    if (df->columnas[columnaIndex].datos)
    {
        free(df->columnas[columnaIndex].datos);
        df->columnas[columnaIndex].datos = NULL;
    }

    // Desplazar las columnas restantes
    for (int i = columnaIndex; i < df->numColumnas - 1; i++)
    {
        df->columnas[i] = df->columnas[i + 1];
    }

    // Reducir el número de columnas
    df->numColumnas--;

    // Liberar la memoria de la última posición de columnas
    df->columnas = realloc(df->columnas, df->numColumnas * sizeof(Columna));
    if (df->columnas == NULL && df->numColumnas > 0)
    {
        printf("\033[0;31mError: No se pudo reasignar memoria para las columnas.\033[0m\n");
        return;
    }

    // Confirmación
    printf("\033[0;32mLa columna '%s' ha sido eliminada correctamente.\033[0m\n", nombreColumna);
}

void quarterColumn(Dataframe *df, const char *nombreColumna, const char *nombreNuevaColumna)
{
    if (!df)
    {
        printf("\033[0;31mError: No hay un dataframe activo.\033[0m\n");
        return;
    }

    // Verificar si la columna original existe y es de tipo FECHA
    int columnaIndex = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0)
        {
            columnaIndex = i;
            break;
        }
    }

    if (columnaIndex == -1)
    {
        printf("\033[0;31mError: La columna '%s' no existe en el dataframe.\033[0m\n", nombreColumna);
        return;
    }

    if (df->columnas[columnaIndex].tipo != FECHA)
    {
        printf("\033[0;31mError: La columna '%s' no es de tipo FECHA.\033[0m\n", nombreColumna);
        return;
    }

    // Verificar si la nueva columna ya existe
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreNuevaColumna) == 0)
        {
            printf("\033[0;31mError: La columna '%s' ya existe en el dataframe.\033[0m\n", nombreNuevaColumna);
            return;
        }
    }

    // Crear espacio para la nueva columna
    Columna *nuevasColumnas = realloc(df->columnas, (df->numColumnas + 1) * sizeof(Columna));
    if (!nuevasColumnas)
    {
        printf("\033[0;31mError: No se pudo asignar memoria para la nueva columna.\033[0m\n");
        return;
    }
    df->columnas = nuevasColumnas;

    // Inicializar la nueva columna
    Columna *nuevaColumna = &df->columnas[df->numColumnas];
    strncpy(nuevaColumna->nombre, nombreNuevaColumna, sizeof(nuevaColumna->nombre) - 1);
    nuevaColumna->nombre[sizeof(nuevaColumna->nombre) - 1] = '\0';
    nuevaColumna->tipo = TEXTO;
    nuevaColumna->datos = calloc(df->numFilas, sizeof(char *));
    nuevaColumna->esNulo = calloc(df->numFilas, sizeof(unsigned char));
    if (!nuevaColumna->datos || !nuevaColumna->esNulo)
    {
        printf("\033[0;31mError: No se pudo asignar memoria para los datos de la nueva columna.\033[0m\n");
        free(nuevaColumna->datos);
        free(nuevaColumna->esNulo);
        return;
    }

    // Procesar las filas
    Columna *columnaOriginal = &df->columnas[columnaIndex];
    for (int i = 0; i < df->numFilas; i++)
    {
        int filaReal = df->indice[i];

        // Manejar valores nulos
        if (columnaOriginal->esNulo[filaReal])
        {
            nuevaColumna->esNulo[filaReal] = 1;
            ((char **)nuevaColumna->datos)[filaReal] = strdup("#N/A");
            continue;
        }

        char *fechaStr = ((char **)columnaOriginal->datos)[filaReal];
        int mes = -1;

        // Validar la fecha y extraer el mes
        if (esFecha(fechaStr))
        {
            mes = atoi(fechaStr + 5); // Extraer el mes del formato YYYY-MM-DD
        }

        // Asignar trimestre basado en el mes
        if (mes >= 1 && mes <= 3)
        {
            nuevaColumna->esNulo[filaReal] = 0;
            ((char **)nuevaColumna->datos)[filaReal] = strdup("Q1");
        }
        else if (mes >= 4 && mes <= 6)
        {
            nuevaColumna->esNulo[filaReal] = 0;
            ((char **)nuevaColumna->datos)[filaReal] = strdup("Q2");
        }
        else if (mes >= 7 && mes <= 9)
        {
            nuevaColumna->esNulo[filaReal] = 0;
            ((char **)nuevaColumna->datos)[filaReal] = strdup("Q3");
        }
        else if (mes >= 10 && mes <= 12)
        {
            nuevaColumna->esNulo[filaReal] = 0;
            ((char **)nuevaColumna->datos)[filaReal] = strdup("Q4");
        }
        else
        {
            nuevaColumna->esNulo[filaReal] = 1;
            ((char **)nuevaColumna->datos)[filaReal] = strdup("#N/A");
        }
    }

    df->numColumnas++;
    printf("\033[0;32mLa columna '%s' ha sido creada correctamente.\033[0m\n", nombreNuevaColumna);
}

// *******************************************************
// *********** FUNCIONES DE FLUJO PRINCIPAL *************
// *******************************************************
void finalizarPrograma()
{
    printf("\033[0;32mEXIT PROGRAM\033[0m\n");
    liberarMemoria();
}

void ejecutarCicloComandos()
{
    char comando[256];
    while (1)
    {
        mostrarPromptDataframe();

        // Leer comando del usuario
        fgets(comando, sizeof(comando), stdin);

        // Eliminar el salto de línea '\n' al final del comando
        comando[strcspn(comando, "\n")] = 0;

        // Manejar comandos específicos
        if (strcmp(comando, "quit") == 0)
        {
            finalizarPrograma();
            break;
        }
        else if (strncmp(comando, "load", 4) == 0)
        {
            manejarComandoLoad(comando + 5);
        }
        else if (strncmp(comando, "view", 4) == 0)
        {
            manejarComandoView(comando + 5);
        }
        else if (strcmp(comando, "meta") == 0)
        {
            manejarComandoMeta();
        } // Comando sort
        else if (strncmp(comando, "sort", 4) == 0)
        {
            manejarComandoSort(comando + 5); // Pasar el comando sin "sort"
        }
        else if (strncmp(comando, "save", 4) == 0)
        {
            manejarComandoSave(comando + 5); // Pasar el comando sin "save"
        }
        else if (strncmp(comando, "filter", 6) == 0)
        {
            manejarComandoFilter(comando + 7); // Pasar el comando sin "filter"
        }
        else if (strncmp(comando, "delnull", 7) == 0)
        {
            manejarComandoDelNull(comando + 8); // Pasar el comando sin "delnull"
        }
        else if (strncmp(comando, "delcolum", 8) == 0)
        {
            manejarComandoDelColum(comando + 9); // Pasar el comando sin "delcolum"
        }
        else if (strncmp(comando, "quarter", 7) == 0)
        {
            manejarComandoQuarter(comando + 8);
        }
        else if (strncmp(comando, "name", 4) == 0)
        {
            manejarComandoName(comando + 5);
        }
        else if (strcmp(comando, "list") == 0)
        {
            manejarComandoList();
        }
        else if (strncmp(comando, "prefix", 6) == 0)
        {
            manejarComandoPrefix(comando + 7);
        }

        else
        {
            manejarComandoDesconocido();
        }
    }
}
