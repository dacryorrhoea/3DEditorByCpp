#pragma once
#include <SDL2/SDL.h>
#include "geometry/scene.h"

void renderInit();
void renderDraw(SDL_Renderer* renderer, SDL_Texture* texture, Scene& scene);