#include "pch.h"

#include "canvas.h"

void Canvas::toRasterizRender(
    std::vector<std::array<float,2>> scene_proj,
    std::vector<std::array<float,2>> ui_buffer
) {
    std::fill_n(pixels_buffer, pix_buf_size, 0x101010FF);

    for (const auto& p : scene_proj) {
        int x = static_cast<int>(p[0]);
        int y = static_cast<int>(p[1]);
        if (x >= 0 && x < W && y >= 0 && y < H)
            pixels_buffer[y * W + x] = 0xFF69B4FF;
    }

    SDL_UpdateTexture(texture, NULL, pixels_buffer, W * sizeof(Uint32));

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    // SDL_RenderCopy(renderer, UI_GetTexture(), NULL, NULL);
    // SDL_RenderCopy(renderer,
    //            UI_GetTextTexture(),
    //            NULL,
    //            UI_GetTextRect());

    SDL_RenderPresent(renderer);
}
