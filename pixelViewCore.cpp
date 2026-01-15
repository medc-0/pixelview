#define SDL_MAIN_HANDLED
#include "pixelViewCore.h"
#include <iostream>
#include <algorithm>

static const int MAX_W = 900;
static const int MAX_H = 600;

ImageViewer::ImageViewer(const std::string& filename)
    : window(nullptr), renderer(nullptr), texture(nullptr), imgW(0), imgH(0), zoom(1.0f), offsetX(0), offsetY(0), dragging(false), lastX(0), lastY(0) {
    loadImage(filename);
}

ImageViewer::~ImageViewer() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void ImageViewer::loadImage(const std::string& filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
        return;
    }

    width = surface->w;
    height = surface->h;

    std::cout << "Loaded image: " << width << "x" << height << " pixels" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    window = SDL_CreateWindow("ImageViewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_FreeSurface(surface);
        SDL_Quit();
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);
}

void ImageViewer::display() {
    if (!texture || width <= 0 || height <= 0) {
        std::cerr << "Error: Image not loaded or invalid dimensions" << std::endl;
        return;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_file.png>" << std::endl;
        return 1;
    }

    ImageViewer viewer(argv[1]);
    viewer.display();

    return 0;
}