#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

#define WIDTH 960
#define HEIGHT 640

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("PixelView v0.1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    Uint8 r,g,b;
    r=0xFF;
    g = b = 0x00;
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);

    SDL_Rect pixel = (SDL_Rect){0, 0, 1, 1};
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            SDL_FillRect(surface, &pixel, color);
        }
    }

    SDL_UpdateWindowSurface(window);
    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}