#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "lib.h"

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

    // Procesar el resto de las filas
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
                    // Limpiar el valor antes de validar
                    inicio[strcspn(inicio, "\r\n")] = '\0';
                    while (*inicio == ' ')
                        inicio++;

                    printf("Validando como TEXTO: '%s' en columna '%s'\n", inicio, df->columnas[i].nombre);
                    if (esTexto(inicio))
                    {
                        ((char **)df->columnas[i].datos)[numFilas] = strdup(inicio);
                        df->columnas[i].esNulo[numFilas] = 0;
                    }
                    else
                    {
                        df->columnas[i].esNulo[numFilas] = 1;
                        printf("\033[0;31mAdvertencia: El valor '%s' en la columna '%s' no coincide con el tipo TEXTO y será marcado como nulo.\033[0m\n", inicio, df->columnas[i].nombre);
                    }
                }

                else
                {
                    df->columnas[i].esNulo[numFilas] = 1; // Tipo incorrecto
                    printf("\033[0;31mAdvertencia: El valor '%s' en la columna '%s' no coincide con el tipo esperado y será marcado como nulo.\033[0m\n", inicio, df->columnas[i].nombre);
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

    df->numFilas = numFilas;

    fclose(archivo);
    return df;
}

// funcion para mostrar las primeras 'n' filas del dataframe
void viewDataframe(Dataframe *df, int n)
{
    if (!df)
    {
        printf("\033[0;31mError: no hay dataframe activo.\033[0m\n");
        return;
    }

    if (n > df->numFilas)
        n = df->numFilas;

    // Mostrar los nombres de las columnas
    for (int i = 0; i < df->numColumnas; i++)
    {
        printf("\033[0;32m%s\t", df->columnas[i].nombre);
    }
    printf("\033[0m\n");

    // Mostrar las primeras 'n' filas
    for (int fila = 0; fila < n; fila++)
    {
        for (int col = 0; col < df->numColumnas; col++)
        {
            if (df->columnas[col].esNulo[fila])
            {
                printf("\033[0;32m#N/A\t");
            }
            else
            {
                if (df->columnas[col].tipo == NUMERICO)
                {
                    printf("\033[0;32m%d\t", ((int *)df->columnas[col].datos)[fila]); // Mostrar enteros
                }
                else if (df->columnas[col].tipo == TEXTO || df->columnas[col].tipo == FECHA)
                {
                    printf("\033[0;32m%s\t", ((char **)df->columnas[col].datos)[fila]); // Mostrar texto o fecha
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