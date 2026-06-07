#include "sup_class/pch.h"
#include "main_classes/sdl_app.h"

void SDLApp::rebuildUI() {
    ui.clearButtons();

    ui.addButton(
        0, 0, 180, 30,
        "change mode",
        [this]() {
            canvas.changeRastMode();
        }
    );
    
    ui.addButton(
        190, 0, 180, 30,
        "merge scene",
        [this]() {
            scene.mergeScene();
            uiDirty = true;
        }
    );

    ui.addButton(
        380, 0, 180, 30,
        "save scene",
        [this]() {
            canvas.changeRastMode();
        }
    );

    ui.addButton(
        570, 0, 180, 30,
        "rollback scene",
        [this]() {
            canvas.changeRastMode();
        }
    );

    ui.addButton(
        0, 45, 180, 30,
        "add model",
        [this]() {
            std::string filename;

            for (const auto& entry :
                std::filesystem::directory_iterator("./data")
            ) {
                std::cout << entry.path().filename().string() << '\n';
            }

            std::cout << "Enter file name: ";
            std::getline(std::cin >> std::ws, filename);

            std::string path = "./data/" + filename;

            scene.addModel(path);
            uiDirty = true;
        }
    );

    ui.addButton(
        0, 80, 180, 30,
        "add cube",
        [this]() {
            scene.addCube();
            uiDirty = true;
        }
    );

    ui.addButton(
        0, 115, 180, 30,
        "add sphere",
        [this]() {
            scene.addSphere();
            uiDirty = true;
        }
    );

    ui.addButton(
        0, 150, 180, 30,
        "add pyramid",
        [this]() {
            scene.addPyramid();
            uiDirty = true;
        }
    );

    int btnY = 200;
    for (Object* obj : scene.getObjects()) {
        int btnX = 0;
        Uint32 objColor = 0x888888FF;
        if (obj == currObject && !objectEdit && !objectScale)
        {
            objColor = 0x33CC33FF;
        }
                
        ui.addButton(
            btnX, btnY, 180, 30,
            obj->getName(),
            [this, obj]() {
                currObject = obj;
                objectEdit = false;
                objectScale = false;
                uiDirty = true;
            },
            objColor
        );

        btnX += 185;

        if (obj->isRenderable())
        { 
            ui.addButton(
                btnX, btnY, 30, 30,
                "X",
                [this, obj]() {
                    if (currObject == obj) {
                        currObject = nullptr;
                    }

                    scene.removeObject(obj);
                    uiDirty = true;
                },
                0xCC4444FF
            );
        }

        btnX += 35;

        if (obj->isSaveble()) {
            ui.addButton(
                btnX, btnY, 30, 30,
                "O",
                [this, obj]() {
                    if (currObject == obj) {
                        currObject = scene.camera;
                    }

                    std::string filename;
                    std::cout << "Enter file name: ";
                    std::getline(std::cin >> std::ws, filename);

                    std::string path = filename + ".obj";

                    Model* model = dynamic_cast<Model*>(obj);
                    Mesh* mesh = dynamic_cast<Mesh*>(obj);
                    if (model && mesh) {
                        model->saveObjectToObjFile(mesh, filename);
                    }
                    scene.removeObject(obj);
                    uiDirty = true;
                },
                0xCCCC33FF
            );

            btnX += 35;
        }


        if (obj == currObject && obj->isEditeble()) {
            Uint32 editColor = (objectEdit && obj == currObject)
                ? 0x33CC33FF
                : 0x888888FF;

            Uint32 scaleColor = (objectScale && obj == currObject)
                ? 0x33CC33FF
                : 0x888888FF;

            ui.addButton(
                btnX, btnY, 30, 30,
                "e",
                [this, obj]() {
                    objectEdit = true;
                    objectScale = false;
                    uiDirty = true;
                },
                editColor
            );

            btnX += 35;

            ui.addButton(
                btnX, btnY, 30, 30,
                "s",
                [this, obj]() {
                    objectEdit = false;
                    objectScale = true;
                    uiDirty = true;
                },
                scaleColor
            );
        }

        btnY += 35;
    }

    ui.build();
    uiDirty = false;
}

void SDLApp::run() {
    if (!currObject) currObject = scene.camera;

    if (uiDirty) rebuildUI();

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
        if (objectEdit) {
            if (
                event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_MIDDLE
            ) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                selectedVerts = scene.camera->selectVertex(
                    event.button.x,
                    event.button.y,
                    currObject
                );
            }

            if (
                event.type == SDL_MOUSEBUTTONUP &&
                event.button.button == SDL_BUTTON_MIDDLE
            ) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                selectedVerts.clear();
            }
        }

        // mouse right click
        if (
            event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_RIGHT
        ) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            objectControl = true;
        }

        if (
            event.type == SDL_MOUSEBUTTONUP &&
            event.button.button == SDL_BUTTON_RIGHT
        ) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            objectControl = false;
        }

        // mouse wheel
        if (event.type == SDL_MOUSEWHEEL && !objectEdit && !objectScale) 
        {
            if (event.wheel.y > 0) {
                currObject->moveForwardBackward(CAM_SPEED*7);
            } else if (event.wheel.y < 0) {
                currObject->moveForwardBackward(-CAM_SPEED*7);
            }
        }

        if (!objectControl && !objectEdit && objectScale) {
            if (event.type == SDL_MOUSEWHEEL) 
            {
                Mesh* mesh = dynamic_cast<Mesh*>(currObject);
                if (!mesh) return;
                if (event.wheel.y > 0) {
                    mesh->scaleMesh(1.1f);
                } else if (event.wheel.y < 0) {
                    mesh->scaleMesh(0.9f);
                }
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
    // WASD movementstate = SDL_GetKeyboardState(NULL);
    if (
        state[SDL_SCANCODE_W] && !objectEdit && !objectScale
    )
    {
        currObject->moveForwardBackward(CAM_SPEED);
    }
    if (
        state[SDL_SCANCODE_A] && !objectEdit && !objectScale
    )
    {
        currObject->moveRightLeft(-CAM_SPEED);
    }
    if (
        state[SDL_SCANCODE_S] && !objectEdit && !objectScale
    )
    {
        currObject->moveForwardBackward(-CAM_SPEED);
    }
    if (
        state[SDL_SCANCODE_D] && !objectEdit && !objectScale
    )
    {
        currObject->moveRightLeft(CAM_SPEED);
    }
    if (
        state[SDL_SCANCODE_SPACE] && !objectEdit && !objectScale
    )
    {
        currObject->moveUpDown(CAM_SPEED);
    }
    if (
        state[SDL_SCANCODE_LSHIFT] && !objectEdit && !objectScale
    )
    {
        currObject->moveUpDown(-CAM_SPEED);
    }
    
    // mouse movement
    int dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    mouse = SDL_GetMouseState(NULL, NULL);

    if (objectControl && !objectEdit && !objectScale) {
        if (dx) currObject->rotateYaw(-0.001f * dx);
        if (dy) currObject->rotatePitch(-0.001f * dy);
    }

    if (!objectControl && objectEdit && !objectScale) {
        Mesh* mesh = dynamic_cast<Mesh*>(currObject);
        if (!mesh) return;

        if (dx) mesh->shiftVertexSetX(-0.001f * dx, selectedVerts);
        if (dy) mesh->shiftVertexSetX(-0.001f * dy, selectedVerts);
    }

    scene.update();

    ui.build();
    
    canvas.toRasterizRender(
        scene.getProjFromCurrCamera(),
        ui.getPixels()
    );
    
    SDL_Delay(10);
}
