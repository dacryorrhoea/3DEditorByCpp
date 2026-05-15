#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class AppContext {
public:
    int window_w = 1000;
    int window_h = 1000;
    SDL_Window* window = nullptr;
    TTF_Font* font = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    bool fail = true;

    AppContext(int w, int h);
    ~AppContext();

    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;
};