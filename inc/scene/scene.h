#pragma once

#include <cmath>
#include "sup_class/custom_types.h"
#include "geometry/mesh.h"
#include "geometry/vertex.h"
#include "objects/camera.h"
#include "objects/model.h"

class Scene {
private:
    std::vector<std::unique_ptr<Object>> objects;
public:
    Camera* camera = nullptr;

    Scene(int w, int h) {
        auto cam = std::make_unique<Camera>(w, h, true);
        camera = cam.get();
        objects.push_back(std::move(cam));
    }

    std::vector<Object*> getObjects() {
        std::vector<Object*> out;
        for (auto& obj : objects)
            out.push_back(obj.get());
        return out;
    }

    // add objects
    void addObject(std::unique_ptr<Object> obj) {
        if (auto cam = dynamic_cast<Camera*>(obj.get())) {
            if (cam->getCamActiveState())
            camera = cam;
        }

        objects.push_back(std::move(obj));
    }

    void addModel(const std::string& path) {
        objects.push_back(std::make_unique<Model>(path));
    }


    // projection work
    const std::vector<Polygon>& getProjFromCurrCamera() {
        return camera->getPolygons();
    }

    void update() {
        std::vector<Object*> sceneObjects;

        for (auto& obj : objects) {
            if (obj->isRenderable()) {
                sceneObjects.push_back(obj.get());
            }
        }

        camera->toProjectingScene(sceneObjects);
    }
};