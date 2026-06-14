#include "suph/pch.h"
#include "main_classes/scene.h"
#include <iostream>
#include <exception>

void Scene::mergeScene() {
    try {
        Mesh merged_mesh;

        for (auto& obj : objects) {
            if (obj->isEditeble()) {
                Mesh* mesh = dynamic_cast<Mesh*>(obj.get());
                Mesh append_mesh(*mesh);
                append_mesh.setTransformedVertices(
                    obj->position,
                    obj->forward,
                    obj->right,
                    obj->up
                );
                merged_mesh += append_mesh;
            }
        }

        Model new_model(merged_mesh);
        addObject(std::make_unique<Model>(merged_mesh));
    } catch (const std::exception& e) {
        std::cerr << "Exception in Scene::mergeScene: "
                  << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception in Scene::mergeScene"
                  << std::endl;
        throw;
    }
}
    
void Scene::saveScene() {

}

void Scene::rollbackScene() {

}
