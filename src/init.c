#include <stdio.h>
#include <stdlib.h>

static int run(const char *cmd) {
    fprintf(stderr, "[init] %s\n", cmd);
    return system(cmd);
}

int main() {
    printf(">>> [INIT] CultraCore PE iniciado <<<\n");

    // Montajes esenciales del entorno en RAM
    run("mount -t proc proc /proc");
    run("mount -t sysfs sysfs /sys");
    run("mount -t devtmpfs devtmpfs /dev");

    printf("CultraCore is loading files...\n");

    // Animación opcional
    run("/bin/bootanim");

    // Intentar instalador gráfico
    int rc = run("/bin/installer_gui");
    if (rc != 0) {
        fprintf(stderr, "[init] installer_gui falló (rc=%d). Abriendo shell.\n", rc);
    }

    // Mantener siempre una shell viva
    while (1) {
        run("/bin/shell");
    }

    return 0; // nunca se alcanza
}
