#pragma once

#include <memory>
#include <iostream>
#include <exception>
#include "suph/all_objects.h"
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
        try {
            if (app_context.fail) {
                running = false;
                return;
            }

            if (!scene.getObjects().empty()) {
                currObject = scene.getObjects()[0];
            } else {
                return;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in SDLApp constructor: "
                      << e.what()
                      << std::endl;
            running = false;
        } catch (...) {
            std::cerr << "Unknown exception in SDLApp constructor"
                      << std::endl;
            running = false;
        }
    }

    bool isRunning() const noexcept {
        return running;
    }

    void run();
};