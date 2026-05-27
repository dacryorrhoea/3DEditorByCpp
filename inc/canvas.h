#pragma once
#include <SDL2/SDL.h>
#include "geometry/containers/polygons.h"

class Canvas {
private:
    int W, H;
    Uint32* pixels_buffer;
    float* z_buffer;
    size_t buffer_size;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    bool rast_mode;
public:
    Canvas(int w, int h, SDL_Renderer* renderer, SDL_Texture* texture)
        : W(w)
        , H(h)
        , pixels_buffer(nullptr)
        , z_buffer(nullptr)
        , buffer_size(static_cast<size_t>(w) * static_cast<size_t>(h))
        , renderer(renderer)
        , texture(texture)
        , rast_mode(true)
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

    void changeRastMode() {
        if (rast_mode) {
            rast_mode = false;
        } else {
            rast_mode = true; 
        }
    }

    void rastFillMode(const PolygonContainer& scene_proj);

    void rastWireframeMode(const PolygonContainer& scene_proj);
    
    void toRasterizRender(
        const PolygonContainer& scene_proj,
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
