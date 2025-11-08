#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf(">>> [INIT] CultraCore init iniciado <<<\n");

    printf(">>> [INIT] Ejecutando bootanim...\n");
    system("/bin/bootanim");

    printf(">>> [INIT] Ejecutando shell...\n");
    system("/bin/shell");

    printf(">>> [INIT] Finalizando init.\n");
    return 0;
}
