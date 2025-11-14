#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf(">>> [INIT] CultraCore init iniciado <<<\n");

    // Animación de arranque
    printf(">>> [INIT] Ejecutando bootanim...\n");
    system("/bin/bootanim");

    // Intentar lanzar instalador gráfico
    printf(">>> [INIT] Intentando ejecutar installer_gui...\n");
    int rc = system("/bin/installer_gui");
    if (rc != 0) {
        printf(">>> [INIT] installer_gui falló o no está disponible (rc=%d).\n", rc);
        printf(">>> [INIT] Ejecutando shell en su lugar...\n");
        system("/bin/shell");
    }

    printf(">>> [INIT] Finalizando init.\n");
    return 0;
}
