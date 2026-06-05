#pragma once

#include <cmath>
#include <algorithm>
#include <memory>
#include "containers/polygons.h"
#include "geometry/mesh.h"
#include "geometry/vertex.h"
#include "objects/all_objects.h"

class Scene {
private:
    std::vector<std::unique_ptr<Object>> objects;
    LightSource* light_src = nullptr;
public:
    Camera* camera = nullptr;

    Scene(int w, int h) {
        auto cam = std::make_unique<Camera>(w, h, true);
        camera = cam.get();
        objects.push_back(std::move(cam));

        auto lsrc = std::make_unique<LightSource>(w, h, true);
        light_src = lsrc.get();
        objects.push_back(std::move(lsrc));

        objects.push_back(std::move(std::make_unique<Ground>()));
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

    void removeObject(Object* target) {
        auto it = std::find_if(
            objects.begin(),
            objects.end(),
            [target](const std::unique_ptr<Object>& obj) {
                return obj.get() == target;
            }
        );

        if (it != objects.end()) {
            objects.erase(it);
        }
    }

    void addModel(const std::string& path) {
        objects.push_back(std::make_unique<Model>(path));
    }

    void addCube() {
        objects.push_back(std::make_unique<Cube>());
    }

    // scene options
    void mergeScene();
    void saveScene();
    void rollbackScene();

    // projection work
    const PolygonContainer& getProjFromCurrCamera() {
        return camera->getPolygons();
    }

    void update() {
        std::vector<Object*> sceneObjects;

        for (auto& obj : objects) {
            if (obj->isRenderable()) {
                sceneObjects.push_back(obj.get());
            }
        }

        camera->toProjectingScene(sceneObjects, *light_src);
    }
};