#include "sup_class/pch.h"
#include "sup_class/app_context.h"

AppContext::AppContext(int w, int h) {
    window_w = w;
    window_h = h;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fail = true;
        return;
    }

    if (TTF_Init() != 0) {
        SDL_Quit();
        fail = true;
        return;
    }

    window = SDL_CreateWindow(
        "(X_X)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_w, window_h, 0
    );
    if (!window) {
        TTF_Quit();
        SDL_Quit();
        fail = true;
        return;
    }

    font = TTF_OpenFont(
        "./data/DroidSans.ttf",
        24
    );
    if (!font) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        fail = true;
        return;
    }

    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_SOFTWARE
    );
    if (!renderer) {
        TTF_CloseFont(font);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        fail = true;
        return;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC,
        window_w, window_h
    );
    if (!texture) { 
        SDL_DestroyRenderer(renderer);
        TTF_CloseFont(font);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        fail = true;
        return;
    }

    fail = false;
}

AppContext::~AppContext() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}