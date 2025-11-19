#include <SDL2/SDL.h>
#include <unistd.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("CultraCore Boot",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    for (int i = 0; i <= 100; i += 5) {
        SDL_SetRenderDrawColor(ren, 18, 18, 18, 255); // fondo
        SDL_RenderClear(ren);

        // barra de progreso
        SDL_Rect bar = { 50, 120, i * 7, 30 };
        SDL_SetRenderDrawColor(ren, 0, 145, 255, 255);
        SDL_RenderFillRect(ren, &bar);

        // texto simulado
        SDL_Rect text = { 50, 60, 700, 20 };
        SDL_SetRenderDrawColor(ren, 180, 180, 180, 255);
        SDL_RenderFillRect(ren, &text);

        SDL_RenderPresent(ren);
        usleep(150000); // animación suave
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
