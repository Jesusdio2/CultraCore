#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf(">>> [INIT] CultraCore PE iniciado <<<\n");

    system("mount -t proc proc /proc");
    system("mount -t sysfs sysfs /sys");
    system("mount -t devtmpfs devtmpfs /dev");

    printf("CultraCore is loading files...\n");

    // animación opcional
    system("/bin/bootanim");

    // instalador opcional
    system("/bin/installer_gui");

    // reemplazar init por la shell
    execl("/bin/shell", "shell", NULL);

    perror("execl");
    return 1;
}
