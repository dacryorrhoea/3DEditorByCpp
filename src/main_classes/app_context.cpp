#include "suph/pch.h"
#include "main_classes/app_context.h"

#include <iostream>
#include <exception>

AppContext::AppContext(int w, int h) {
    try {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fail = true;
            return;
        }

        if (TTF_Init() != 0) {
            SDL_Quit();
            fail = true;
            return;
        }

        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);

        window_w = (w > 0) ? w : dm.w;
        window_h = (h > 0) ? h : dm.h;

        window = SDL_CreateWindow(
            "3DEditorByCpp",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            window_w, window_h, 0
        );
        if (!window) {
            TTF_Quit();
            SDL_Quit();
            fail = true;
            return;
        }

        font = TTF_OpenFont("./data/DroidSans.ttf", 24);
        if (!font) {
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            fail = true;
            return;
        }

        renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_SOFTWARE
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
    } catch (const std::exception& e) {
        std::cerr << "Exception in AppContext constructor: "
                  << e.what()
                  << std::endl;
        fail = true;
    } catch (...) {
        std::cerr << "Unknown exception in AppContext constructor"
                  << std::endl;
        fail = true;
    }
}

AppContext::~AppContext() noexcept {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}