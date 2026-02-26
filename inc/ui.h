#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

void UI_Init(SDL_Renderer* renderer, TTF_Font* font, int W, int H);
void UI_Destroy();

SDL_Texture* UI_GetTexture();
SDL_Texture* UI_GetTextTexture();
SDL_Rect* UI_GetTextRect();

bool UI_HitTest(int x, int y);