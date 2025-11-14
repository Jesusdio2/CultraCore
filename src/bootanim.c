#include <stdio.h>
#include <unistd.h>

int main() {
    printf(">>> [BOOTANIM] CultraCore is loading files...\n");

    for (int i = 0; i <= 100; i += 10) {
        printf("\r[%-10s] %3d%%", 
               "##########" + (10 - i/10), i);
        fflush(stdout);
        sleep(1); // simula carga
    }

    printf("\n>>> [BOOTANIM] Carga completa.\n");
    return 0;
}
