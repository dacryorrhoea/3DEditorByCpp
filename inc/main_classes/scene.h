#pragma once

#include <cmath>
#include <algorithm>
#include <memory>
#include <iostream>
#include <exception>
#include "containers/polygons.h"
#include "suph/all_objects.h"

class Scene {
private:
    std::vector<std::unique_ptr<Object>> objects;
    LightSource* light_src = nullptr;

    enum class Type : std::uint32_t {
        Object = 0,
        Camera,
        LightSource,
        Model,
        Ground,
        Cube,
        Sphere,
        Pyramid
    };

public:
    Camera* camera = nullptr;

    Scene(int w, int h) {
        try {
            auto cam = std::make_unique<Camera>(w, h, true);
            camera = cam.get();
            objects.push_back(std::move(cam));

            auto lsrc = std::make_unique<LightSource>(w, h, true);
            light_src = lsrc.get();
            objects.push_back(std::move(lsrc));

            objects.push_back(std::move(std::make_unique<Ground>()));
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene constructor"
                      << std::endl;
            throw;
        }
    }

    std::vector<Object*> getObjects() {
        std::vector<Object*> out;
        for (auto& obj : objects)
            out.push_back(obj.get());
        return out;
    }

    void addObject(std::unique_ptr<Object> obj) {
        try {
            if (auto cam = dynamic_cast<Camera*>(obj.get())) {
                if (cam->getCamActiveState())
                    camera = cam;
            }
            objects.push_back(std::move(obj));
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene::addObject: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene::addObject"
                      << std::endl;
            throw;
        }
    }

    void removeObject(Object* target) {
        try {
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
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene::removeObject: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene::removeObject"
                      << std::endl;
            throw;
        }
    }

    void addModel(const std::string& path) {
        try {
            objects.push_back(std::make_unique<Model>(path));
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene::addModel: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene::addModel"
                      << std::endl;
            throw;
        }
    }

    void addCube() {
        try {
            objects.push_back(std::make_unique<Cube>());
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene::addCube: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene::addCube"
                      << std::endl;
            throw;
        }
    }

    void addPyramid() {
        try {
            objects.push_back(std::make_unique<Pyramid>());
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene::addPyramid: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene::addPyramid"
                      << std::endl;
            throw;
        }
    }

    void addSphere() {
        try {
            objects.push_back(std::make_unique<Sphere>());
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene::addSphere: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene::addSphere"
                      << std::endl;
            throw;
        }
    }

    // scene options
    void mergeScene();
    void saveScene();
    void rollbackScene();

    const PolygonContainer& getProjFromCurrCamera() const noexcept {
        return camera->getPolygons();
    }

    void update() {
        try {
            std::vector<Object*> sceneObjects;
            for (auto& obj : objects) {
                if (obj->isRenderable()) {
                    sceneObjects.push_back(obj.get());
                }
            }
            camera->toProjectingScene(sceneObjects, *light_src);
        } catch (const std::exception& e) {
            std::cerr << "Exception in Scene::update: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Scene::update"
                      << std::endl;
            throw;
        }
    }
};