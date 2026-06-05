#include "sup_class/pch.h"

#include "main_classes/canvas.h"

void Canvas::rastFillMode(const PolygonContainer& scene_proj) {
    const int SCALE = 2;

    for (const auto& pol : scene_proj) {
        int x1 = pol.p1.x / SCALE;
        int y1 = pol.p1.y / SCALE;
        int x2 = pol.p2.x / SCALE;
        int y2 = pol.p2.y / SCALE;
        int x3 = pol.p3.x / SCALE;
        int y3 = pol.p3.y / SCALE;

        int dx13 = x1 - x3;
        int dx32 = x3 - x2;
        int dy13 = y1 - y3;
        int dy23 = y2 - y3;

        float ap = static_cast<float>(dy23 * dx13 + dx32 * dy13);
        if (ap == 0.0f) continue;

        int dx21 = x2 - x1;
        int dy21 = y2 - y1;
        int dy32 = y3 - y2;

        if (ap < 0.0f) {
            ap = -ap;
            dx13 = -dx13; dx32 = -dx32; dx21 = -dx21;
            dy13 = -dy13; dy23 = -dy23; dy21 = -dy21; dy32 = -dy32;
        }

        int minX = std::max(0, std::min(std::min(x1, x2), x3));
        int minY = std::max(0, std::min(std::min(y1, y2), y3));
        int maxX = std::min(W / SCALE - 1, std::max(std::max(x1, x2), x3));
        int maxY = std::min(H / SCALE - 1, std::max(std::max(y1, y2), y3));

        int e1_row = (minX - x1) * dy21 + y1 * dx21;
        int e2_row = (minX - x2) * dy32 + y2 * dx32;
        int e3_row = (minX - x3) * dy13 + y3 * dx13;

        float invAp = 1.0f / ap;
        float wk1 = invAp * pol.p1.inv_z;
        float wk2 = invAp * pol.p2.inv_z;
        float wk3 = invAp * pol.p3.inv_z;

        int e1_start = minY * dx21 - e1_row;
        int e2_start = minY * dx32 - e2_row;
        int e3_start = minY * dx13 - e3_row;

        for (int y = minY; y <= maxY; ++y) {
            int e1 = e1_start;
            int e2 = e2_start;
            int e3 = e3_start;

            int pos = y * (W / SCALE) + minX;
            for (int x = minX; x <= maxX; ++x) {
                if (e1 >= 0 && e2 >= 0 && e3 >= 0) {
                    float inv_z = e2 * wk1 + e3 * wk2 + e1 * wk3;
                    if (z_buffer[pos] < inv_z) {
                        z_buffer[pos] = inv_z;
                        pixels_buffer[pos] = pol.flat_shading;
                    }
                }

                ++pos;
                e1 -= dy21;
                e2 -= dy32;
                e3 -= dy13;
            }

            e1_start += dx21;
            e2_start += dx32;
            e3_start += dx13;
        }
    }

    for (int y = H - 1; y >= 0; --y) {
        int sy = y / SCALE;
        for (int x = W - 1; x >= 0; --x) {
            int sx = x / SCALE;
            pixels_buffer[y * W + x] = pixels_buffer[sy * (W / SCALE) + sx];
        }
    }
}

void Canvas::rastWireframeMode(const PolygonContainer& scene_proj) {
        Uint32 color = 0;

        for (auto& pol : scene_proj) {
            if (color != pol.solid_color) {
                Uint8 r = (pol.solid_color >> 24) & 0xFF;
                Uint8 g = (pol.solid_color >> 16) & 0xFF;
                Uint8 b = (pol.solid_color >> 8)  & 0xFF;
                Uint8 a = pol.solid_color & 0xFF;

                SDL_SetRenderDrawColor(renderer, r, g, b, a);
            }
            SDL_RenderDrawLine(
                renderer, pol.p1.x, pol.p1.y, pol.p2.x, pol.p2.y
            );
            SDL_RenderDrawLine(
                renderer, pol.p2.x, pol.p2.y, pol.p3.x, pol.p3.y
            );
            SDL_RenderDrawLine(
                renderer, pol.p3.x, pol.p3.y, pol.p1.x, pol.p1.y
            );
        }
}

void Canvas::toRasterizRender(
    const PolygonContainer& scene_proj,
    Uint32* ui_buffer
) {
    if (!ui_buffer) return;

    SDL_RenderClear(renderer);
    std::fill_n(pixels_buffer, buffer_size, 0x101010FF);
    std::fill_n(z_buffer, buffer_size, 0.0f);

    if (rast_mode) {
        rastFillMode(scene_proj);

        for (int i = 0; i < buffer_size; ++i) {
            Uint32 px = ui_buffer[i];

            if ((px >> 24) == 0)
                continue;

            pixels_buffer[i] = px;
        }

        SDL_UpdateTexture(
            texture, nullptr, pixels_buffer, W * sizeof(Uint32)
        );
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    } else {

        for (int i = 0; i < buffer_size; ++i) {
            Uint32 px = ui_buffer[i];

            if ((px >> 24) == 0)
                continue;

            pixels_buffer[i] = px;
        }
        
        SDL_UpdateTexture(
            texture, nullptr, pixels_buffer, W * sizeof(Uint32)
        );
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        rastWireframeMode(scene_proj);
    }


    SDL_RenderPresent(renderer);
}
