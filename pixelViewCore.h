#ifndef PIXELVIEWCORE_H
#define PIXELVIEWCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class ImageViewer {
public:
    ImageViewer(const std::string& filename);
    ~ImageViewer();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;

    int imgW;
    int imgH;

    float zoom;
    float offsetX;
    float offsetY;

    bool dragging;
    int lastX;
    int lastY;

    void loadImage(const std::string& filename);
    void fitToWindow();
    void render();
};

#endif
