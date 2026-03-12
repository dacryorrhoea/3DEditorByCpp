#include "pch.h"

#include "render.h"
#include "geometry/scene.h"
#include "ui.h"

static Uint32* pixels;
static int W, H;

void Render_Draw(SDL_Renderer* renderer, SDL_Texture* texture, Scene& scene) {
    W=1400;
    H=1000;

    // std::vector<std::array<float,2>> arr;
    // arr = scene.getProjected();
    // for (const auto& p : arr) {
    //     std::cout << "x: " << p[0] << ", y: " << p[1] << "\n";
    // }

    pixels = new Uint32[W * H];
    
    for (int i = 0; i < W * H; ++i)
        pixels[i] = 0x030303FF;

    // заливаем точки из projectedPoints
    for (const auto& p : scene.getProjected()) {
        int x = static_cast<int>(p[0]);
        int y = static_cast<int>(p[1]);
        if (x >= 0 && x < W && y >= 0 && y < H)
            pixels[y * W + x] = 0xFF0000FF; // красный пиксель
    }

    // for (int i = 0; i < W*H; ++i) {
    //     if (pixels[i] == 0xFF0000FF) {
    //         std::cout << pixels[i] << "\n";
    //     }
    // }

    SDL_UpdateTexture(texture, NULL, pixels, W * sizeof(Uint32));

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderCopy(renderer, UI_GetTexture(), NULL, NULL);
    SDL_RenderCopy(renderer,
               UI_GetTextTexture(),
               NULL,
               UI_GetTextRect());

    SDL_RenderPresent(renderer);
}

