#include <SDL.h>
#include <SDL_ttf.h>

#include "scene.h"
#include "ui.h"
#include "render.h"
// #include "window.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;

    if (TTF_Init() != 0) { SDL_Quit(); return 1; }

    SDL_Window* window = SDL_CreateWindow(
        "(X_X)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1400, // width
        1000, // height
        0
    );
    if (!window) { SDL_Quit(); return 1; }


    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_SOFTWARE
    );
    if (!renderer) { SDL_DestroyWindow(window); SDL_Quit(); return 1; }


    TTF_Font* font = TTF_OpenFont(
        "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans.ttf",
        32
    );
    if (!font) { TTF_Quit(); SDL_Quit(); return 1; }


    UI_Init(renderer, font, 1400, 1000);

    
    std::string ObjPath = "../cube.obj";  
    Scene_Init(renderer, 1400, 1000, );


    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                continue;
            }

            if (
                event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT
            ) {
                if (UI_HitTest(event.button.x, event.button.y)) {
                    running = false;
                    continue;
                }
            }

            if (
                event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE
            ) {
                    running = false;
                    continue;
            }
        }

        Render_Draw(renderer);
        SDL_Delay(10);
    }

    UI_Destroy();
    Scene_Destroy();

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}