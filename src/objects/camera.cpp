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
            const float nearPlane = 0.1f;

            if (v1.z <= nearPlane || v2.z <= nearPlane || v3.z <= nearPlane)
                continue;

            Vertex n;
            n = (v2 - v1).cross(v3 - v1);

            if (std::abs(n.x) + std::abs(n.y) + std::abs(n.z) < 1e-6)
                continue;

            if (n.dot(v1) >= 0)
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

            pol.color = obj->color;

            polygons.push_back(pol);
        }

        const float axisLen = 1.7f;
        const float axisThick = 0.03f;

        struct AxisData {
            Vertex dir;
            Vertex side1;
            Vertex side2;
            Uint32 color;
        };

        AxisData axes[3] = {
            { obj->right,   obj->up,      obj->forward, 0xFF0000FF },
            { obj->up,      obj->right,   obj->forward, 0x00FF00FF },
            { obj->forward, obj->right,   obj->up,      0x0000FFFF }
        };

        for (int a = 0; a < 3; ++a)
        {
            AxisData& axis = axes[a];

            Vertex start = obj->position;
            Vertex end   = obj->position + axis.dir * axisLen;

            Vertex s1 = axis.side1 * axisThick;
            Vertex s2 = axis.side2 * axisThick;

            std::vector<Vertex> boxWorld = {
                start - s1 - s2,
                start + s1 - s2,
                start + s1 + s2,
                start - s1 + s2,

                end - s1 - s2,
                end + s1 - s2,
                end + s1 + s2,
                end - s1 + s2
            };

            std::vector<Vertex> boxCam(8);

            for (int i = 0; i < 8; ++i)
            {
                Vertex v = boxWorld[i] - position;

                boxCam[i].x = v.dot(right);
                boxCam[i].y = v.dot(up);
                boxCam[i].z = v.dot(forward);
            }

            std::vector<Faces> boxFaces = {
                {0,1,2}, {0,2,3},
                {4,6,5}, {4,7,6},
                {0,4,5}, {0,5,1},
                {3,2,6}, {3,6,7},
                {1,5,6}, {1,6,2},
                {0,3,7}, {0,7,4}
            };

            for (Faces& face : boxFaces)
            {
                Vertex v1 = boxCam[face.f1];
                Vertex v2 = boxCam[face.f2];
                Vertex v3 = boxCam[face.f3];

                const float nearPlane = 0.1f;
                if (v1.z <= nearPlane || v2.z <= nearPlane || v3.z <= nearPlane)
                    continue;

                Vertex n = (v2 - v1).cross(v3 - v1);

                if (std::abs(n.x) + std::abs(n.y) + std::abs(n.z) < 1e-6f)
                    continue;

                if (n.dot(v1) >= 0)
                    continue;

                Polygon pol;

                pol.p1.x = static_cast<int>((v1.x / v1.z) * focal + HW + 0.5f);
                pol.p1.y = static_cast<int>(-(v1.y / v1.z) * focal + HH + 0.5f);
                pol.p1.inv_z = 1.0f / v1.z;

                pol.p2.x = static_cast<int>((v2.x / v2.z) * focal + HW + 0.5f);
                pol.p2.y = static_cast<int>(-(v2.y / v2.z) * focal + HH + 0.5f);
                pol.p2.inv_z = 1.0f / v2.z;

                pol.p3.x = static_cast<int>((v3.x / v3.z) * focal + HW + 0.5f);
                pol.p3.y = static_cast<int>(-(v3.y / v3.z) * focal + HH + 0.5f);
                pol.p3.inv_z = 1.0f / v3.z;

                pol.color = axis.color;

                polygons.push_back(pol);
            }
        }
    }
}