#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    system("mount -t proc proc /proc");
    system("mount -t sysfs sysfs /sys");
    system("mount -t devtmpfs devtmpfs /dev");

    // Mostrar barra de progreso gráfica
    system("/bin/bootanim");

    // Lanzar instalador gráfico
    system("/bin/installer_gui");

    // Si falla, abrir shell
    execl("/bin/shell", "shell", NULL);
    perror("execl");
    return 1;
}

