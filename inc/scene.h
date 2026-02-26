#pragma once
#include <SDL.h>

struct Vertex {
    float x, y, z;
}

struct Faces {
    int vert1, vert2;
}

class Camera {
private:
    Vertex camera_position;
};

class Viewport {
public:
    int width;
    int height;
    Uint32* pixels;
};

void Scene_Init(SDL_Renderer* renderer, int W, int H);
void Scene_Destroy();
SDL_Texture* Scene_GetTexture();