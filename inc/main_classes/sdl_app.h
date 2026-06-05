#pragma once

#include <memory>
#include "objects/all_objects.h"
#include "main_classes/app_context.h"
#include "main_classes/scene.h"
#include "main_classes/ui.h"
#include "main_classes/canvas.h"

class SDLApp {
private:
    const float CAM_SPEED = 1.0f;

    AppContext app_context;
    Canvas canvas;
    Scene scene;
    UI ui;

    SDL_Event event;
    const Uint8* state;
    Uint32 mouse;
    Object* currObject;
    std::vector<int> selectedVerts;

    bool running;
    bool objectControl;
    bool objectEdit;
    bool objectScale;
    bool uiDirty;

    void rebuildUI();

public:
    SDLApp()
        : app_context()
        , canvas(
            app_context.window_w,
            app_context.window_h,
            app_context.renderer,
            app_context.texture
        )
        , scene(
            app_context.window_w,
            app_context.window_h
        )
        , ui(
            app_context.window_w,
            app_context.window_h,
            app_context.font
        )
        , state(SDL_GetKeyboardState(NULL))
        , mouse(0)
        , currObject(nullptr)
        , selectedVerts()
        , running(true)
        , objectControl(false)
        , objectEdit(false)
        , objectScale(false)
        , uiDirty(true)
    {
        if (app_context.fail) {
            running = false;
            return;
        }

        if (!scene.getObjects().empty()) {
            currObject = scene.getObjects()[0];
        } else {
            return;
        }
    }

    bool isRunning() {
        return running;
    }

    void run();

    void stop() {
        //
    }
};