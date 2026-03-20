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
        int minX = std::max(0, std::min(std::min(pol.p1.x, pol.p2.x), pol.p3.x));
        int minY = std::max(0, std::min(std::min(pol.p1.y, pol.p2.y), pol.p3.y));
        int maxX = std::min(W - 1, std::max(std::max(pol.p1.x, pol.p2.x), pol.p3.x));
        int maxY = std::min(H - 1, std::max(std::max(pol.p1.y, pol.p2.y), pol.p3.y));

        int ap = (pol.p2.y - pol.p3.y)*(pol.p1.x - pol.p3.x) + (pol.p3.x - pol.p2.x)*(pol.p1.y - pol.p3.y);

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                int e1 = (x - pol.p1.x)*(pol.p2.y - pol.p1.y) - (y - pol.p1.y)*(pol.p2.x - pol.p1.x);
                int e2 = (x - pol.p2.x)*(pol.p3.y - pol.p2.y) - (y - pol.p2.y)*(pol.p3.x - pol.p2.x);
                int e3 = (x - pol.p3.x)*(pol.p1.y - pol.p3.y) - (y - pol.p3.y)*(pol.p1.x - pol.p3.x);

                if ((e1 >= 0 && e2 >= 0 && e3 >= 0) || (e1 <= 0 && e2 <= 0 && e3 <= 0)) {
                    float w1 = -static_cast<float>(e2) / ap;
                    float w2 = -static_cast<float>(e3) / ap;
                    float w3 = -static_cast<float>(e1) / ap;

                    float inv_z = w1 * pol.p1.inv_z + w2 * pol.p2.inv_z + w3 * pol.p3.inv_z;

                    int pos = y * W + x;
                    if (z_buffer[pos] < inv_z) {
                        z_buffer[pos] = inv_z;
                    }
                }
            }
        }
    }   


    SDL_UpdateTexture(texture, NULL, pixels_buffer, W * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (const Polygon& pol : scene_proj) {
        int pos1 = pol.p1.y * W + pol.p1.x;
        if (pos1 >= buffer_size) continue;
        if (pol.p1.inv_z < z_buffer[pos1]) continue;

        int pos2 = pol.p2.y * W + pol.p2.x;
        if (pos2 >= buffer_size) continue;
        if (pol.p2.inv_z < z_buffer[pos2]) continue;

        SDL_RenderDrawLine(renderer, pol.p1.x, pol.p1.y, pol.p2.x, pol.p2.y);

        int pos3 = pol.p3.y * W + pol.p3.x;
        if (pos3 >= buffer_size) continue;
        if (pol.p3.inv_z < z_buffer[pos3]) continue;

        SDL_RenderDrawLine(renderer, pol.p2.x, pol.p2.y, pol.p3.x, pol.p3.y);
        SDL_RenderDrawLine(renderer, pol.p3.x, pol.p3.y, pol.p1.x, pol.p1.y);
    }


    // for (int i = 0; i < buffer_size; ++i) {
    //     if (z_buffer[i] > 0) {
    //         pixels_buffer[i] = 0xFF69B4FF;
    //     }
    // }


    // SDL_UpdateTexture(texture, NULL, pixels_buffer, W * sizeof(Uint32));
    // SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);
}
