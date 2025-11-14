#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char cmd[256];
    printf(">>> [SHELL] CultraCore shell iniciada <<<\n");

    while (1) {
        printf("cultracore> ");
        fflush(stdout);

        if (!fgets(cmd, sizeof(cmd), stdin)) break;
        cmd[strcspn(cmd, "\n")] = 0; // quitar salto de línea

        if (strcmp(cmd, "exit") == 0) {
            printf(">>> [SHELL] Saliendo...\n");
            break;
        }

        if (strlen(cmd) > 0) system(cmd);
    }

    return 0;
}
