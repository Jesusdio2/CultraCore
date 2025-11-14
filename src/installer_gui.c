// installer_gui.c
// CultraCore – Instalador gráfico híbrido (mouse + táctil) usando SDL2
// Compilación (Ubuntu):
//   sudo apt-get update && sudo apt-get install -y libsdl2-dev
//   gcc installer_gui.c -o installer_gui -lSDL2
// Ejecútalo como root dentro del initramfs.

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// ----------------------------- Utilidades UI -----------------------------

typedef struct {
    SDL_Rect rect;
    const char *label;
    bool highlighted;
} Button;

static SDL_Window   *g_win  = NULL;
static SDL_Renderer *g_ren  = NULL;
static int           g_w    = 800;
static int           g_h    = 600;

static Button btn_install;
static Button btn_advanced;
static Button btn_reboot;
static Button btn_quit;

static bool   has_touch = false;
static bool   has_mouse = false;

// Colores
static SDL_Color COLOR_BG        = { 18, 18, 18, 255 };
static SDL_Color COLOR_PANEL     = { 28, 28, 28, 255 };
static SDL_Color COLOR_ACCENT    = { 0, 145, 255, 255 };
static SDL_Color COLOR_BTN       = { 36, 36, 36, 255 };
static SDL_Color COLOR_BTN_HOVER = { 0, 95, 185, 255 };
static SDL_Color COLOR_TEXT      = { 235, 235, 235, 255 };
static SDL_Color COLOR_SUBTEXT   = { 180, 180, 180, 255 };

static void set_draw_color(SDL_Color c) { SDL_SetRenderDrawColor(g_ren, c.r, c.g, c.b, c.a); }
static void fill_rect(SDL_Rect r, SDL_Color c) { set_draw_color(c); SDL_RenderFillRect(g_ren, &r); }

static void draw_border(SDL_Rect r, SDL_Color c, int th) {
    set_draw_color(c);
    SDL_Rect top    = { r.x, r.y, r.w, th };
    SDL_Rect bottom = { r.x, r.y + r.h - th, r.w, th };
    SDL_Rect left   = { r.x, r.y, th, r.h };
    SDL_Rect right  = { r.x + r.w - th, r.y, th, r.h };
    SDL_RenderFillRect(g_ren, &top);
    SDL_RenderFillRect(g_ren, &bottom);
    SDL_RenderFillRect(g_ren, &left);
    SDL_RenderFillRect(g_ren, &right);
}

// Placeholder de texto: barra para evitar dependencia de SDL_ttf
static void draw_label(SDL_Rect r, const char *text, SDL_Color c) {
    int pad = r.h / 4;
    SDL_Rect bar = { r.x + pad, r.y + (r.h/2 - pad/3), r.w - 2*pad, pad/2 };
    fill_rect(bar, c);
    (void)text;
}

static bool point_in_rect(int x, int y, SDL_Rect r) {
    return (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h);
}

// Cursor simple (triángulo)
static void draw_cursor(int mx, int my) {
    set_draw_color(COLOR_TEXT);
    for (int i = 0; i < 12; ++i) {
        SDL_RenderDrawLine(g_ren, mx, my + i, mx + 12 - i, my + 4 + i);
        SDL_RenderDrawLine(g_ren, mx + i/2, my, mx + 4 - i/2, my + 12);
    }
}

// ------------------------ Lógica “stubs” de instalación -------------------

static void log_step(const char *msg) { fprintf(stderr, "[installer] %s\n", msg); }

static int run_cmd(const char *cmd) {
    log_step(cmd);
    int rc = system(cmd);
    if (rc != 0) fprintf(stderr, "[installer] comando falló (rc=%d): %s\n", rc, cmd);
    return rc;
}

static int do_install_basic(void) {
    log_step("Instalación básica en /dev/sda1 (ejemplo)");
    if (run_cmd("mkfs.ext4 -F /dev/sda1") != 0) return -1;
    if (run_cmd("mkdir -p /mnt/target") != 0) return -1;
    if (run_cmd("mount /dev/sda1 /mnt/target") != 0) return -1;
    // Asegúrate de incluir /rootfs en tu initramfs
    if (run_cmd("cp -a /rootfs/* /mnt/target/") != 0) return -1;
    run_cmd("mkdir -p /mnt/target/boot");
    log_step("Bootloader: integra GRUB o Syslinux según tu diseño (pendiente).");
    if (run_cmd("sync") != 0) return -1;
    if (run_cmd("umount /mnt/target") != 0) return -1;
    log_step("Instalación completada.");
    return 0;
}

static void do_advanced_menu(void) {
    log_step("Menú avanzado: seleccionar disco, particiones, FS, bootloader (pendiente).");
}

// ----------------------------- Layout y dibujo ----------------------------

static void layout_ui(bool touch_mode) {
    int panel_pad = 24;
    int btn_h = touch_mode ? 88 : 64;
    int btn_w = g_w - (panel_pad * 2);
    int y = panel_pad + 64;

    btn_install.rect   = (SDL_Rect){ panel_pad, y, btn_w, btn_h }; y += btn_h + 18;
    btn_advanced.rect  = (SDL_Rect){ panel_pad, y, btn_w, btn_h }; y += btn_h + 18;
    btn_reboot.rect    = (SDL_Rect){ panel_pad, y, btn_w, btn_h }; y += btn_h + 18;
    btn_quit.rect      = (SDL_Rect){ panel_pad, y, btn_w, btn_h };

    btn_install.label  = "Instalar en disco";
    btn_advanced.label = "Opciones avanzadas";
    btn_reboot.label   = "Reiniciar";
    btn_quit.label     = "Salir";
}

static void draw_button(Button *b) {
    SDL_Color base = b->highlighted ? COLOR_BTN_HOVER : COLOR_BTN;
    fill_rect(b->rect, base);
    draw_border(b->rect, COLOR_PANEL, 2);
    SDL_Rect lab = { b->rect.x + 16, b->rect.y + b->rect.h/2 - 8, b->rect.w - 32, 16 };
    draw_label(lab, b->label, COLOR_TEXT);
}

static void draw_header(bool touch_mode) {
    SDL_Rect top = { 0, 0, g_w, 56 };
    fill_rect(top, COLOR_PANEL);
    SDL_Rect lab = { 16, 16, 220, 20 };
    draw_label(lab, "CultraCore Installer", COLOR_TEXT);
    SDL_Rect mode = { g_w - 220, 16, 200, 20 };
    draw_label(mode, touch_mode ? "Modo táctil" : "Modo mouse", COLOR_SUBTEXT);
}

static void render_frame(int mx, int my, bool touch_mode) {
    fill_rect((SDL_Rect){0,0,g_w,g_h}, COLOR_BG);
    draw_header(touch_mode);

    SDL_Rect panel = { 12, 72, g_w - 24, g_h - 84 };
    draw_border(panel, COLOR_PANEL, 2);

    draw_button(&btn_install);
    draw_button(&btn_advanced);
    draw_button(&btn_reboot);
    draw_button(&btn_quit);

    if (!touch_mode && has_mouse) draw_cursor(mx, my);
    SDL_RenderPresent(g_ren);
}

// ------------------------------- Eventos ----------------------------------

static void update_hover(int mx, int my, bool touch_mode) {
    if (touch_mode) {
        btn_install.highlighted  = false;
        btn_advanced.highlighted = false;
        btn_reboot.highlighted   = false;
        btn_quit.highlighted     = false;
        return;
    }
    btn_install.highlighted  = point_in_rect(mx, my, btn_install.rect);
    btn_advanced.highlighted = point_in_rect(mx, my, btn_advanced.rect);
    btn_reboot.highlighted   = point_in_rect(mx, my, btn_reboot.rect);
    btn_quit.highlighted     = point_in_rect(mx, my, btn_quit.rect);
}

static void handle_click_at(int x, int y) {
    if (point_in_rect(x, y, btn_install.rect)) { do_install_basic(); return; }
    if (point_in_rect(x, y, btn_advanced.rect)) { do_advanced_menu(); return; }
    if (point_in_rect(x, y, btn_reboot.rect)) {
        run_cmd("reboot -f || echo 1 > /proc/sys/kernel/sysrq; echo b > /proc/sysrq-trigger");
        return;
    }
    if (point_in_rect(x, y, btn_quit.rect)) {
        run_cmd("echo 'Saliendo al shell…'; /bin/sh");
        return;
    }
}

// ------------------------------- Arranque ---------------------------------

static void detect_input_capabilities(void) {
    has_touch = (SDL_GetNumTouchDevices() > 0);
    has_mouse = true; // habilitar cursor por defecto; se ajusta con eventos
    SDL_ShowCursor(has_touch ? SDL_DISABLE : SDL_ENABLE);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init falló: %s\n", SDL_GetError());
        return 1;
    }

    g_win = SDL_CreateWindow("CultraCore Installer",
                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             g_w, g_h, SDL_WINDOW_SHOWN);
    if (!g_win) {
        fprintf(stderr, "SDL_CreateWindow falló: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    g_ren = SDL_CreateRenderer(g_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_ren) {
        fprintf(stderr, "SDL_CreateRenderer falló: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_win);
        SDL_Quit();
        return 1;
    }

    detect_input_capabilities();
    bool touch_mode = has_touch;
    layout_ui(touch_mode);

    bool running = true;
    int  mx = g_w/2, my = g_h/2;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEMOTION:
                    has_mouse = true;
                    mx = e.motion.x;
                    my = e.motion.y;
                    update_hover(mx, my, touch_mode);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    has_mouse = true;
                    handle_click_at(e.button.x, e.button.y);
                    break;

                case SDL_FINGERDOWN: {
                    has_touch = true;
                    touch_mode = true;
                    layout_ui(touch_mode);
                    int tx = (int)(e.tfinger.x * g_w);
                    int ty = (int)(e.tfinger.y * g_h);
                    handle_click_at(tx, ty);
                    break;
                }

                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        g_w = e.window.data1;
                        g_h = e.window.data2;
                        layout_ui(touch_mode);
                    }
                    break;

                default:
                    break;
            }
        }
        render_frame(mx, my, touch_mode);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(g_ren);
    SDL_DestroyWindow(g_win);
    SDL_Quit();
    return 0;
}
