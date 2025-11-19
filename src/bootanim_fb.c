#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>   // <--- importante para ioctl()

int main() {
    int fb = open("/dev/fb0", O_RDWR);
    if (fb < 0) { perror("open fb0"); return 1; }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("FBIOGET_VSCREENINFO");
        return 1;
    }
    if (ioctl(fb, FBIOGET_FSCREENINFO, &finfo)) {
        perror("FBIOGET_FSCREENINFO");
        return 1;
    }

    long screensize = vinfo.yres_virtual * finfo.line_length;
    char *fbp = (char*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

    if ((intptr_t)fbp == -1) { perror("mmap"); return 1; }

    // Fondo negro
    memset(fbp, 0, screensize);

    // Barra azul simple
    int bar_height = 50;
    int bar_width = vinfo.xres_virtual / 2;
    int x_offset = vinfo.xres_virtual / 4;
    int y_offset = vinfo.yres_virtual / 2;

    for (int y = y_offset; y < y_offset + bar_height; y++) {
        for (int x = x_offset; x < x_offset + bar_width; x++) {
            long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                            (y + vinfo.yoffset) * finfo.line_length;
            // Azul (BGRA)
            fbp[location] = 255;      // Blue
            fbp[location+1] = 0;      // Green
            fbp[location+2] = 0;      // Red
            fbp[location+3] = 0;      // Alpha
        }
    }

    munmap(fbp, screensize);
    close(fb);
    return 0;
}
