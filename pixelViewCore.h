#ifndef PIXELVIEWCORE_H
#define PIXELVIEWCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

class ImageViewer {
public:
    ImageViewer(const std::string& filename);
    ~ImageViewer();
    void display();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;
    void loadImage(const std::string& filename);
};

#endif
