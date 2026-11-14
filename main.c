#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

// Función principal
int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            activarModoVerbose();
        }
    }
    inicializarPrograma();
    ejecutarCicloComandos();
    return 0;
}