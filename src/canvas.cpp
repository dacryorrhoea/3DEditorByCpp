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
        int dx13 = pol.p1.x - pol.p3.x;
        int dx32 = pol.p3.x - pol.p2.x;
        int dy13 = pol.p1.y - pol.p3.y;
        int dy23 = pol.p2.y - pol.p3.y;

        float ap = static_cast<float>((dy23)*(dx13) + (dx32)*(dy13));
        if (ap == 0) continue;

        int dx21 = pol.p2.x - pol.p1.x;
        int dy21 = pol.p2.y - pol.p1.y;
        int dy32 = pol.p3.y - pol.p2.y;

        int minX = std::max(0, std::min(std::min(pol.p1.x, pol.p2.x), pol.p3.x));
        int minY = std::max(0, std::min(std::min(pol.p1.y, pol.p2.y), pol.p3.y));
        int maxX = std::min(W - 1, std::max(std::max(pol.p1.x, pol.p2.x), pol.p3.x));
        int maxY = std::min(H - 1, std::max(std::max(pol.p1.y, pol.p2.y), pol.p3.y));

        int e1_rows = (minX - pol.p1.x)*(dy21) + pol.p1.y*(dx21);
        int e2_rows = (minX - pol.p2.x)*(dy32) + pol.p2.y*(dx32);
        int e3_rows = (minX - pol.p3.x)*(dy13) + pol.p3.y*(dx13);

        double wk1 = (1.0 / ap) * pol.p1.inv_z;
        double wk2 = (1.0 / ap) * pol.p2.inv_z;
        double wk3 = (1.0 / ap) * pol.p3.inv_z;

        int minYW = minY * W;

        for (int x = minX; x <= maxX; ++x) {  
            int e1 = minY*(dx21) - e1_rows;
            int e2 = minY*(dx32) - e2_rows;
            int e3 = minY*(dx13) - e3_rows;
         
            int pos = minYW + x;
            for (int y = minY; y <= maxY; ++y) {
                if ((e1 >= 0 && e2 >= 0 && e3 >= 0) || (e1 <= 0 && e2 <= 0 && e3 <= 0)) {
                    double inv_z = e2*wk1 + e3*wk2 + e1*wk3;
                    if (z_buffer[pos] < inv_z) {
                        z_buffer[pos] = inv_z;
                    }
                }
                pos += W;
                e1 += dx21;
                e2 += dx32;
                e3 += dx13;
            }
            e1_rows += dy21;
            e2_rows += dy32;
            e3_rows += dy13;
        }
    }   


    SDL_UpdateTexture(texture, NULL, pixels_buffer, W * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    for (const Polygon& pol : scene_proj) {
        SDL_RenderDrawLine(renderer, pol.p1.x, pol.p1.y, pol.p2.x, pol.p2.y);
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
