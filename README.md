# dataframe-C
# Descripción

Este proyecto consiste en desarrollar un programa en **C** que gestione un conjunto de datos estructurados en forma de _dataframe_ a través de una interfaz de línea de comandos (CLI). El objetivo es implementar a bajo nivel la estructura y las operaciones básicas para manejar datos organizados en filas y columnas, soportando diferentes tipos de datos (texto, numéricos y fechas) y detectando valores nulos o anómalos.

---

## Funcionalidades

- **Carga de Datos**: Permite cargar datos desde archivos CSV, mostrando errores en la CLI si se detectan valores incorrectos o nulos.
- **Operaciones Básicas**:
  - Obtener estadísticas básicas.
  - Filtrar filas según condiciones específicas.
  - Generar nuevas columnas derivadas.
- **Gestión de Memoria**: El programa debe liberar toda la memoria dinámica al finalizar.

---

## Tipos de Datos Soportados

- **Texto**: Cadenas de caracteres.
- **Números**: Enteros y decimales.
- **Fechas**: Formato `YYYY-MM-DD`.

---

## Estructuras de Datos

- **`TipoDato`**: Enumeración para representar los diferentes tipos de datos.
- **`Columna`**: Estructura para representar una columna del dataframe.
- **`Dataframe`**: Estructura principal que representa un conjunto de datos.
- **`Lista`**: Estructura para manejar múltiples dataframes.

---

## Comandos Disponibles

- `quit`: Finaliza el programa.
- `load <nombre_fichero>`: Carga un archivo CSV.
- `meta`: Muestra los metadatos del dataframe activo.
- `view [n]`: Muestra las primeras `n` filas del dataframe.
- `sort <nombre_columna> [asc/des]`: Ordena las filas según la columna indicada.
- `save <nombre_fichero>`: Guarda el dataframe en un archivo CSV.
- `filter <nombre_columna> eq/neq/gt/lt <valor>`: Filtra las filas según la condición especificada.
- `delnull <nombre_columna>`: Elimina filas con valores nulos en la columna indicada.
- `delcolum <nombre_columna>`: Elimina la columna indicada.
- `quarter <nombre_columna> <nombre_nueva_columna>`: Crea una nueva columna con el trimestre correspondiente a las fechas.

---

## Requisitos Generales

- **Gestión de Errores**:
  - Mensajes de error en **rojo**.
  - Mensajes de éxito en **verde**.
- **Implementación en C**:
  - El proyecto debe estar constituido por tres archivos:
    - `main.c`
    - `lib.h`
    - `lib.c`
