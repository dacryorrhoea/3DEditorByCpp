#pragma once
#include <SDL2/SDL.h>
#include "sup_class/custom_types.h"

class Canvas {
private:
    int W, H;
    Uint32* pixels_buffer;
    float* z_buffer;
    size_t buffer_size;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
public:
    Canvas(int w, int h, SDL_Renderer* renderer, SDL_Texture* texture)
        : W(w)
        , H(h)
        , pixels_buffer(nullptr)
        , z_buffer(nullptr)
        , buffer_size(static_cast<size_t>(w) * static_cast<size_t>(h))
        , renderer(renderer)
        , texture(texture)
    {
        pixels_buffer = new Uint32[buffer_size];
        z_buffer = new float[buffer_size];
        std::fill_n(z_buffer, buffer_size, 0.0f);
        std::fill_n(pixels_buffer, buffer_size, 0u);
    }
    
    ~Canvas() {
        delete[] pixels_buffer;
        pixels_buffer = nullptr;
    }
    
    void toRasterizRender(
        std::vector<Polygon> scene_proj,
        Uint32* ui_buffer
    );

    void toUpdate(int w, int h) {
        W = w;
        H = h;
        buffer_size = static_cast<size_t>(w) * static_cast<size_t>(h);
        delete[] pixels_buffer;
        pixels_buffer = new Uint32[buffer_size];
        std::fill_n(pixels_buffer, buffer_size, 0u);
    }
};
