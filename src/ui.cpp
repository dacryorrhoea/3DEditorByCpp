#include "ui.h"

static SDL_Texture* texture;
static SDL_Texture* textTexture;
static SDL_Rect textRect;

static int bx, by, bw, bh;
static int W, H;

void UI_Init(SDL_Renderer* renderer, TTF_Font* font, int w, int h) {
    W = w;
    H = h;

    bx = W/2 - 60;
    by = H - 80;
    bw = 120;
    bh = 40;

    Uint32* pixels = new Uint32[W * H];

    for (int i = 0; i < W * H; i++)
        pixels[i] = 0x00000000;

    for (int y = by; y < by + bh; y++)
        for (int x = bx; x < bx + bw; x++)
            pixels[y * W + x] = 0x888888FF;

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC,
        W, H
    );
                                 
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_UpdateTexture(texture, NULL, pixels, W * sizeof(Uint32));
    delete[] pixels;

    SDL_Color white = {255,255,255,255};
    SDL_Surface* textSurface =
        TTF_RenderText_Blended(font, "exit", white);

    textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    textRect = {bx + bw/4, by,
                textSurface->w,
                textSurface->h};

    SDL_FreeSurface(textSurface);
}

SDL_Texture* UI_GetTexture() { return texture; }
SDL_Texture* UI_GetTextTexture() { return textTexture; }
SDL_Rect* UI_GetTextRect() { return &textRect; }

bool UI_HitTest(int x, int y) {
    return (x >= bx && x <= bx + bw && y >= by && y <= by + bh);
}

void UI_Destroy() {
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(texture);
}