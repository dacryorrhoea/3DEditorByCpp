#pragma once
#include <SDL2/SDL.h>

class Canvas {
private:
    int W, H;
    Uint32* pixels_buffer;
    size_t pix_buf_size;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
public:
    Canvas(int w, int h, SDL_Renderer* renderer, SDL_Texture* texture)
        : W(w)
        , H(h)
        , pixels_buffer(nullptr)
        , pix_buf_size(static_cast<size_t>(w) * static_cast<size_t>(h))
        , renderer(renderer)
        , texture(texture)
    {
        pixels_buffer = new Uint32[pix_buf_size];
        std::fill_n(pixels_buffer, pix_buf_size, 0u);
    }
    
    ~Canvas() {
        delete[] pixels_buffer;
        pixels_buffer = nullptr;
    }
    
    void toRasterizRender(
        std::vector<std::array<float,2>> scene_proj,
        std::vector<std::array<float,2>> ui_buffer
    );

    void toUpdate(int w, int h) {
        W = w;
        H = h;
        pix_buf_size = static_cast<size_t>(w) * static_cast<size_t>(h);
        delete[] pixels_buffer;
        pixels_buffer = new Uint32[pix_buf_size];
        std::fill_n(pixels_buffer, pix_buf_size, 0u);
    }
};
