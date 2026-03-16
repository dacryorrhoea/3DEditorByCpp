#pragma once

#include "geometry/mesh.h"
#include "geometry/vertex.h"


struct Camera {
    Vertex position;
    Vertex forward;
    Vertex right;
    Vertex up;

    Camera()
        : position(0.5f, 0.5f, 5.0f)
        , forward(0.0f, 0.0f, -1.0f)
        , right(1.0f, 0.0f, 0.0f)
        , up(0.0f, 1.0f, 0.0f)
    {}

    void moveForwardBackward(float dist) {
        position.x += forward.x * dist;
        position.y += forward.y * dist;
        position.z += forward.z * dist;
    }

    void moveRightLeft(float dist) {
        position.x += right.x * dist;
        position.y += right.y * dist;
        position.z += right.z * dist;
    }

    void moveUpDown(float dist) {
        position.x += up.x * dist;
        position.y += up.y * dist;
        position.z += up.z * dist;
    }

    void rotateYaw(float angle);
    void rotatePitch(float angle);
};


class Scene {
private:
    int W;
    int H;
    float focal;
    Camera camera;
    std::vector<std::array<float,2>> projectedPoints;
public:
    std::vector<Mesh> meshes;

    Scene(int w, int h)
        : W(w)
        , H(h)
        , focal( (W * 0.5f) / std::tan( (80.0f * 3.14159265358979323846f / 180.0f) * 0.5f ) )
        , camera()
    {}

    void projectMeshes();

    void addMesh(const Mesh& mesh) {
        meshes.push_back(mesh);
    }

    const std::vector<std::array<float,2>>& getProjected() const {
        return projectedPoints;
    }
};