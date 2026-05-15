#include "sup_class/pch.h"

#include "objects/camera.h"
#include "geometry/vertex.h"

void Camera::toProjectingScene(const std::vector<Object*>& objects) {
    polygons.clear();

    const float HW = W * 0.5f;
    const float HH = H * 0.5f;

    for (Object* obj : objects)
    {
        auto* mesh = dynamic_cast<Mesh*>(obj);
        // if (!mesh) continue;

        std::vector<Vertex> verticesWorld;
        mesh->getTransformedVertices(
            obj->position,
            obj->forward,
            obj->right,
            obj->up,
            verticesWorld
        );
        std::vector<Faces> &faces = mesh->getFaces();
        std::vector<Vertex> verticesCamSpace(verticesWorld.size());

        for (size_t i = 0; i < verticesWorld.size(); ++i)
        {
            Vertex v = verticesWorld[i] - position;

            verticesCamSpace[i].x = v.dot(right);
            verticesCamSpace[i].y = v.dot(up);
            verticesCamSpace[i].z = v.dot(forward);
        }

        for (Faces &face : faces)
        {
            Vertex v1 = verticesCamSpace[face.f1];
            Vertex v2 = verticesCamSpace[face.f2];
            Vertex v3 = verticesCamSpace[face.f3];

            // отсечение некоторого количества граней
            if (std::abs(v1.z) < 0.01f || std::abs(v2.z) < 0.01f || std::abs(v3.z) < 0.01f)
                continue;

            Vertex n;
            n = (v2 - v1).cross(v3 - v1);

            if (std::abs(n.x) + std::abs(n.y) + std::abs(n.z) < 1e-6)
                continue;

            if (n.dot((v1 + v2 + v3) * (1.0f / 3.0f)) < 0)
                continue;

            Polygon pol;
            pol.p1.x = static_cast<int>((v1.x / v1.z) * focal + HW + 0.5f);
            pol.p1.y = static_cast<int>(-(v1.y / v1.z) * focal + HH + 0.5f);
            pol.p1.inv_z = 1 / v1.z;

            pol.p2.x = static_cast<int>((v2.x / v2.z) * focal + HW + 0.5f);
            pol.p2.y = static_cast<int>(-(v2.y / v2.z) * focal + HH + 0.5f);
            pol.p2.inv_z = 1 / v2.z;

            pol.p3.x = static_cast<int>((v3.x / v3.z) * focal + HW + 0.5f);
            pol.p3.y = static_cast<int>(-(v3.y / v3.z) * focal + HH + 0.5f);
            pol.p3.inv_z = 1 / v3.z;

            pol.color = 0xFF69B4FF;

            polygons.push_back(pol);
        }
    }
}