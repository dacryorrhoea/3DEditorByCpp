#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class AppContext {
public:
    int window_w;
    int window_h;
    SDL_Window* window = nullptr;
    TTF_Font* font = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    bool fail = true;

    AppContext(int w = 0, int h = 0);
    ~AppContext();
};