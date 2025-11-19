#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char cmd[256];

    printf(">>> [SHELL] CultraCore shell iniciada <<<\n");

    while (1) {
        printf("cultracore> ");
        fflush(stdout);

        // leer comando
        if (!fgets(cmd, sizeof(cmd), stdin)) {
            break; // EOF o error
        }

        // quitar salto de línea
        cmd[strcspn(cmd, "\n")] = 0;

        // comando "exit" para salir
        if (strcmp(cmd, "exit") == 0) {
            printf(">>> [SHELL] Saliendo...\n");
            break;
        }

        // ejecutar comando si no está vacío
        if (strlen(cmd) > 0) {
            system(cmd);
        }
    }

    return 0;
}
