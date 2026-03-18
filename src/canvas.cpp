#include "pch.h"

#include "canvas.h"

void Canvas::toRasterizRender(
    std::vector<Polygon> scene_proj,
    std::vector<Polygon> ui_buffer
) {
    SDL_RenderClear(renderer);
    std::fill_n(pixels_buffer, buffer_size, 0x101010FF);
    std::fill_n(z_buffer, buffer_size, 0.0f);

    for (const Polygon& pol : scene_proj) {
        
    }   

    SDL_UpdateTexture(texture, NULL, pixels_buffer, W * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
