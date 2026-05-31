#include "sup_class/pch.h"

#include "objects/camera.h"
#include "geometry/vertex.h"

void Camera::toProjectingScene(const std::vector<Object*>& objects, const LightSource& light_src) {
    polygons.Clear();

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
            Vertex v1 = verticesWorld[face.f1];
            Vertex v2 = verticesWorld[face.f2];
            Vertex v3 = verticesWorld[face.f3];

            Vertex e1 = v2 - v1;
            Vertex e2 = v3 - v1;

            Vertex normal = e1.cross(e2);
            normal.normalized();

            Vertex lightDir = light_src.forward;
            lightDir.normalized();

            float brightness = abs(normal.dot(lightDir));

            brightness = std::max(0.2f, brightness);

            Uint8 r = (obj->color >> 24) & 0xFF;
            Uint8 g = (obj->color >> 16) & 0xFF;
            Uint8 b = (obj->color >> 8)  & 0xFF;
            Uint8 a = obj->color & 0xFF;

            r = Uint8(r * brightness);
            g = Uint8(g * brightness);
            b = Uint8(b * brightness);

            Uint32 litColor = (r << 24) | (g << 16) | (b << 8) | a;

            polygons.Add(
                verticesCamSpace[face.f1],
                verticesCamSpace[face.f2],
                verticesCamSpace[face.f3],
                focal, HW, HH,
                obj->color,
                litColor
            );
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
                polygons.Add(
                    boxCam[face.f1],
                    boxCam[face.f2],
                    boxCam[face.f3],
                    focal, HW, HH,
                    axis.color,
                    axis.color
                );
            }
        }
    }
}