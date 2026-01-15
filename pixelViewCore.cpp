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

    int x = 50;
    int y = 50;
    SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
    SDL_FillRect(surface, &pixel, color);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}