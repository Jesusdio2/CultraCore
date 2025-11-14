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

    // Opcional: si incluyes udev en el PE, arráncalo aquí
    // run("/sbin/udevd --daemon"); run("udevadm trigger"); run("udevadm settle");

    // Animación (opcional, ya en RAM)
    run("/bin/bootanim");

    // Lanzar instalador gráfico; si falla, abrir shell
    int rc = run("/bin/installer_gui");
    if (rc != 0) {
        fprintf(stderr, "[init] installer_gui falló (rc=%d). Abriendo shell.\n", rc);
        run("/bin/shell");
    }

    // Mantener sesión si el instalador termina
    run("/bin/shell");
    return 0;
}
