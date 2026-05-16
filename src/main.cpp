#include "sup_class/pch.h"

#include <memory>
#include "sup_class/app_context.h"
#include "objects/cube.h"
#include "objects/model.h"
#include "scene/scene.h"
#include "ui.h"
#include "canvas.h"

const float CAM_SPEED = 1.0f;

int main() {
    AppContext app_context(2000, 1200);
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

    UI ui(
        app_context.window_w,
        app_context.window_h,
        app_context.font
    );

    scene.addCube();
    // scene.addModel("./cube.obj");

    bool running = true;
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    Uint32 mouse;
    bool cameraControl = false;

    
    ui.clearButtons();

    Object* currObject = scene.getObjects()[0];

    int btnY = 40;
    for (Object* obj : scene.getObjects()) {
        ui.addButton(
            0, btnY, 200, 30,
            obj->getName(),
            [&currObject, obj]() {
                currObject = obj;
            }
        );
        btnY += 34;
    }

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
                ui.hitTest(
                    event.button.x,
                    event.button.y
                );
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
                    currObject->moveForwardBackward(CAM_SPEED*7);
                } else if (event.wheel.y < 0) {
                    currObject->moveForwardBackward(-CAM_SPEED*7);
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

        // camera control
        // WASD movement
        state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W]) {
            currObject->moveForwardBackward(CAM_SPEED);
        }
        if (state[SDL_SCANCODE_A]) {
            currObject->moveRightLeft(-CAM_SPEED);
        }
        if (state[SDL_SCANCODE_S]) {
            currObject->moveForwardBackward(-CAM_SPEED);
        }
        if (state[SDL_SCANCODE_D]) {
            currObject->moveRightLeft(CAM_SPEED);
        }
        if (state[SDL_SCANCODE_SPACE]) {
            currObject->moveUpDown(CAM_SPEED);
        }
        if (state[SDL_SCANCODE_LSHIFT]) {
            currObject->moveUpDown(-CAM_SPEED);
        }

        // mouse movement
        int dx, dy;
        SDL_GetRelativeMouseState(&dx, &dy);
        mouse = SDL_GetMouseState(NULL, NULL);

        if (cameraControl) {
            if (dx) currObject->rotateYaw(-0.001f * dx);
            if (dy) currObject->rotatePitch(-0.001f * dy);
        }

        scene.update();

        ui.build();
        
        canvas.toRasterizRender(scene.getProjFromCurrCamera(), ui.getPixels());
        
        SDL_Delay(10);
    }

    return 0;
}
