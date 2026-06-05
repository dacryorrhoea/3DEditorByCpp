#include "sup_class/pch.h"

#include "scene/scene.h"


void Scene::mergeScene() {
    Mesh merged_mesh;

    for (auto& obj : objects)
    {
        if (obj->isEditeble())
        {
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
}
    
void Scene::saveScene() {

}

void Scene::rollbackScene() {

}
