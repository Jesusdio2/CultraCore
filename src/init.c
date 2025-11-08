#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf(">>> CultraCore init iniciado <<<\n");

    // Lanzar animación de arranque
    system("system/bin/bootanim");

    // Lanzar shell interactivo
    system("system/bin/shell");

    return 0;
}
