#include "render.h"
#include "scene.h"
#include "ui.h"

void Render_Draw(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, Scene_GetTexture(), NULL, NULL);
    SDL_RenderCopy(renderer, UI_GetTexture(), NULL, NULL);
    SDL_RenderCopy(renderer,
               UI_GetTextTexture(),
               NULL,
               UI_GetTextRect());

    SDL_RenderPresent(renderer);
}