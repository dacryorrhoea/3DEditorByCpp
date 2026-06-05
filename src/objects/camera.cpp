#include "sup_class/pch.h"

#include "objects/camera.h"


void Camera::getVerticesCamSpace(
    const VertexContainer& vertW,
    VertexContainer& vertCamSpace
) {
    for (size_t i = 0; i < vertW.Size(); ++i)
    {
        Vertex v = vertW[i] - position;

        vertCamSpace[i].x = v.dot(right);
        vertCamSpace[i].y = v.dot(up);
        vertCamSpace[i].z = v.dot(forward);
    }
}

std::vector<int> Camera::selectVertex(int x, int y, Object* obj)
{
    std::vector<int> result;

    auto* mesh = dynamic_cast<Mesh*>(obj);
    if (!mesh) return result;

    const float HW = W * 0.5f;
    const float HH = H * 0.5f;

    VertexContainer verticesWorld;
        mesh->getTransformedVertices(
            obj->position,
            obj->forward,
            obj->right,
            obj->up,
            verticesWorld
        );

    VertexContainer verticesCamSpace(verticesWorld.Size());
    getVerticesCamSpace(
        verticesWorld,
        verticesCamSpace
    );

    auto project = [&](const Vertex& v, float& sx, float& sy) -> bool
    {
        if (v.z <= 0.0001f) return false;
        sx = focal * v.x / v.z + HW;
        sy = HH - focal * v.y / v.z;
        return true;
    };

    int bestIdx = -1;
    float bestDist2 = 30.0f * 30.0f;

    for (size_t i = 0; i < verticesCamSpace.Size(); ++i)
    {
        float sx, sy;
        if (!project(verticesCamSpace[i], sx, sy)) continue;

        float dx = float(x) - sx;
        float dy = float(y) - sy;
        float dist2 = dx * dx + dy * dy;

        if (dist2 < bestDist2)
        {
            bestDist2 = dist2;
            bestIdx = (int)i;
        }
    }

    if (bestIdx == -1) return result;

    result.push_back(bestIdx);

    FaceContainer& faces = mesh->getFaces();
    for (const Face& face : faces)
    {
        if (face.f1 == bestIdx || face.f2 == bestIdx || face.f3 == bestIdx)
        {
            if (
                std::find(
                    result.begin(), result.end(), face.f1
                ) == result.end()
            ) {
                result.push_back(face.f1);
            }

            if (
                std::find(
                    result.begin(), result.end(), face.f2
                ) == result.end()
            ) {
                result.push_back(face.f2);
            }
    
            if (
                std::find(
                    result.begin(), result.end(), face.f3
                ) == result.end()
            ) {
                result.push_back(face.f3);
            }
            }
    }

    return result;
}

void Camera::toProjectingScene(
    const std::vector<Object*>& objects,
    const LightSource& light_src
) {
    polygons.Clear();

    const float HW = W * 0.5f;
    const float HH = H * 0.5f;

    for (Object* obj : objects)
    {
        auto* mesh = dynamic_cast<Mesh*>(obj);

        VertexContainer verticesWorld;
        mesh->getTransformedVertices(
            obj->position,
            obj->forward,
            obj->right,
            obj->up,
            verticesWorld
        );

        VertexContainer verticesCamSpace(verticesWorld.Size());
        getVerticesCamSpace(
            verticesWorld,
            verticesCamSpace
        );

        FaceContainer &faces = mesh->getFaces();
        for (Face &face : faces)
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
                face.f1,
                face.f2,
                face.f3,
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

            VertexContainer boxWorld = {
                start - s1 - s2,
                start + s1 - s2,
                start + s1 + s2,
                start - s1 + s2,

                end - s1 - s2,
                end + s1 - s2,
                end + s1 + s2,
                end - s1 + s2
            };

            VertexContainer boxCam(8);

            for (int i = 0; i < 8; ++i)
            {
                Vertex v = boxWorld[i] - position;

                boxCam[i].x = v.dot(right);
                boxCam[i].y = v.dot(up);
                boxCam[i].z = v.dot(forward);
            }

            FaceContainer boxFaces = {
                {0,1,2}, {0,2,3},
                {4,6,5}, {4,7,6},
                {0,4,5}, {0,5,1},
                {3,2,6}, {3,6,7},
                {1,5,6}, {1,6,2},
                {0,3,7}, {0,7,4}
            };

            for (Face& face : boxFaces)
            {
                polygons.Add(
                    boxCam[face.f1],
                    boxCam[face.f2],
                    boxCam[face.f3],
                    face.f1,
                    face.f2,
                    face.f3,
                    focal, HW, HH,
                    axis.color,
                    axis.color
                );
            }
        }
    }
}