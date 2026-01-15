#define SDL_MAIN_HANDLED
#include "pixelViewCore.h"
#include <iostream>
#include <algorithm>

static const int MAX_W = 900;
static const int MAX_H = 600;

ImageViewer::ImageViewer(const std::string& filename)
    : window(nullptr), renderer(nullptr), texture(nullptr), imgW(0), imgH(0), zoom(1.0f), offsetX(0), offsetY(0), dragging(false), lastX(0), lastY(0) {
    
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF);

    window = SDL_CreateWindow("PixelView v1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAX_W, MAX_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    loadImage(filename);
    fitToWindow();
}

ImageViewer::~ImageViewer() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void ImageViewer::loadImage(const std::string& filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
        return;
    }

    imgW = surface->w;
    imgH = surface->h;

    // std::cout << "image: " << width << "x" << height << " pixels" << std::endl;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void ImageViewer::fitToWindow()
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    float sx = static_cast<float>(w / h);
    float sy = static_cast<float>(imgW / imgH);

    zoom = std::min(sx, sy);
    offsetX = (w - imgW * zoom) * 0.5f;
    offsetY = (h - imgH * zoom) * 0.5f;
}

void ImageViewer::render() 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_FRect dst;
    dst.x = offsetX;
    dst.y = offsetY;
    dst.w = imgW * zoom;
    dst.h = imgH * zoom;

    SDL_RenderCopyF(renderer, texture, nullptr, &dst);
    SDL_RenderPresent(renderer);
}

void ImageViewer::run() 
{
    bool running = true;
    SDL_Event event;

    while (running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
                fitToWindow();

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
                if (event.key.keysym.sym == SDLK_f) fitToWindow();
                if (event.key.keysym.sym == SDLK_r) {
                    zoom = 1.0f;
                    offsetX = offsetY = 0;
                }
            }

            if (event.type == SDL_MOUSEWHEEL) {
                float prev = zoom;
                zoom *= (event.wheel.y > 0) ? 1.1f : 0.9f;
                zoom = std::clamp(zoom, 0.05f, 50.0f);

                int mx, my;
                SDL_GetMouseState(&mx, &my);

                offsetX = mx - (mx - offsetX) * (zoom / prev);
                offsetY = my - (my - offsetY) * (zoom / prev);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                dragging = true;
                lastX = event.button.x;
                lastY = event.button.y;
            }

            if (event.type == SDL_MOUSEBUTTONUP)
                dragging = false;

            if (event.type == SDL_MOUSEMOTION && dragging) {
                offsetX += event.motion.x - lastX;
                offsetY += event.motion.y - lastY;
                lastX = event.motion.x;
                lastY = event.motion.y;
            }
        }

        render();
    }
}
 
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: pixelview <image>" << std::endl;
        return 1;
    }

    ImageViewer viewer(argv[1]);
    viewer.run();
    return 0;
}