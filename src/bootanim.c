#include <stdio.h>
#include <unistd.h>

int main() {
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
        printf("\r%s", frames[i]);
        fflush(stdout);
        sleep(1);
    }
    printf("\nBootanim terminado\n");
    return 0;
}
