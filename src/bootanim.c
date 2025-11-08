#include <stdio.h>
#include <unistd.h>

int main() {
    printf(">>> [BOOTANIM] Animación de arranque iniciada <<<\n");

    const char *frames[] = {
        "[=     ]",
        "[==    ]",
        "[===   ]",
        "[====  ]",
        "[===== ]",
        "[======]"
    };
    int num_frames = sizeof(frames) / sizeof(frames[0]);

    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == NULL) {
            fprintf(stderr, ">>> [BOOTANIM] Error: frame %d no se pudo leer\n", i);
            return 1;
        }
        printf("\r%s", frames[i]);
        fflush(stdout);
        sleep(1);
    }

    printf("\n>>> [BOOTANIM] Animación terminada <<<\n");
    return 0;
}
