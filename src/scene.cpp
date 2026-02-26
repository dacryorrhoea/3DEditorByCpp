#include "scene.h"

static SDL_Texture* texture;
static Uint32* pixels;
static int W, H;

void Scene_Init(SDL_Renderer* renderer, int w, int h) {
    W = w;
    H = h;

    pixels = new Uint32[W * H];

    for (int i = 0; i < W * H; i++)
        pixels[i] = 0x030303FF;

    int qx = W/2-50, qy = H/2-50, qsize = 100;

    for (int y = qy; y < qy + qsize; y++)
        for (int x = qx; x < qx + qsize; x++)
            pixels[y * W + x] = 0xFF0000FF;

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC,
        W, H
    );

    SDL_UpdateTexture(texture, NULL, pixels, W * sizeof(Uint32));
}

SDL_Texture* Scene_GetTexture() {
    return texture;
}

void Scene_Destroy() {
    delete[] pixels;
    SDL_DestroyTexture(texture);
}