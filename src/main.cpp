#include "pch.h"

#include "geometry/mesh.h"
#include "app_context.h"
#include "scene.h"
#include "ui.h"
#include "canvas.h"

const float CAM_SPEED = 1.0f;

int main() {
    AppContext app_context(1400, 1000);
    if (app_context.fail) return 1;

    Canvas canvas(
        app_context.window_w,
        app_context.window_h,
        app_context.renderer,
        app_context.texture
    );
    Scene scene(
        app_context.window_w,
        app_context.window_h
    );

    Mesh mesh;
    try {
        mesh.loadMeshFromFile("./cube.obj");
        std::cout << "mesh loaded successfully\n";
    }
    catch (const std::exception& e) {
        std::cout << "error: " << e.what() << "\n";
    }


    scene.addMesh(mesh);


    bool running = true;
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    Uint32 mouse;
    bool cameraControl = false;

    while (running) {
        while (SDL_PollEvent(&event)) {
            // system close button
            if (event.type == SDL_QUIT) {
                running = false;
                continue;
            }

            // mouse left click
            if (
                event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT
            ) {
                // if (UI_HitTest(event.button.x, event.button.y)) {
                //     running = false;
                //     continue;
                // }
            }

            // mouse middle click
            if (
                event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_MIDDLE
            ) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                cameraControl = true;
            }

            if (
                event.type == SDL_MOUSEBUTTONUP &&
                event.button.button == SDL_BUTTON_MIDDLE
            ) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                cameraControl = false;
            }

            // mouse wheel
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    scene.camera.moveForwardBackward(CAM_SPEED*7);
                } else if (event.wheel.y < 0) {
                    scene.camera.moveForwardBackward(-CAM_SPEED*7);
                }
            }

            // exit
            if (
                event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE
            ) {
                    running = false;
                    continue;
            }
        }

        // WASD movement
        state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W]) {
            scene.camera.moveForwardBackward(CAM_SPEED);
        }
        if (state[SDL_SCANCODE_A]) {
            scene.camera.moveRightLeft(-CAM_SPEED);
        }
        if (state[SDL_SCANCODE_S]) {
            scene.camera.moveForwardBackward(-CAM_SPEED);
        }
        if (state[SDL_SCANCODE_D]) {
            scene.camera.moveRightLeft(CAM_SPEED);
        }
        if (state[SDL_SCANCODE_SPACE]) {
            scene.camera.moveUpDown(CAM_SPEED);
        }
        if (state[SDL_SCANCODE_LSHIFT]) {
            scene.camera.moveUpDown(-CAM_SPEED);
        }

        // camera control
        int dx, dy;
        SDL_GetRelativeMouseState(&dx, &dy);
        mouse = SDL_GetMouseState(NULL, NULL);

        if (cameraControl) {
            if (dx) scene.camera.rotateYaw(-0.001f * dx);
            if (dy) scene.camera.rotatePitch(-0.001f * dy);
        }

        scene.toProjectingScene();
        
        canvas.toRasterizRender(scene.getPolygons(), scene.getPolygons());
        
        SDL_Delay(10);
    }

    return 0;
}
