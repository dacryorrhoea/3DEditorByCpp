#pragma once

#include <cmath>
#include <array>
#include <vector>
#include "mesh.h"
#include "vertex.h"


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

    void rotateYaw(float angle) {
        const float EPS = 1e-6f;
        float sinA = std::sin(angle);
        float cosA = std::cos(angle);
        float t = 1.0f - cosA;

        // 1 on 1000000000000000000000
        float ul = std::sqrt(up.x*up.x + up.y*up.y + up.z*up.z);
        if (ul > EPS && std::fabs(ul - 1.0f) > EPS) {
            up.x /= ul;
            up.y /= ul;
            up.z /= ul;
        }

        // create Rodrigues matrix
        std::array<float,16> m = {
            t*up.x*up.x + cosA,      t*up.x*up.y - sinA*up.z, t*up.x*up.z + sinA*up.y, 0.0f,
            t*up.x*up.y + sinA*up.z, t*up.y*up.y + cosA,      t*up.y*up.z - sinA*up.x, 0.0f,
            t*up.x*up.z - sinA*up.y, t*up.y*up.z + sinA*up.x, t*up.z*up.z + cosA,      0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        forward.transformV(m);
        right.transformV(m);

        // orthonormalization aka Gram–Schmidt
        // step 1
        // 'fabs' check absolute eq
        float fl = std::sqrt(forward.x*forward.x + forward.y*forward.y + forward.z*forward.z);
        if (fl > EPS && std::fabs(fl - 1.0f) > EPS) {
            forward.x /= fl;
            forward.y /= fl;
            forward.z /= fl;
        }

        // step 2
        right.x = forward.y*up.z - forward.z*up.y;
        right.y = forward.z*up.x - forward.x*up.z;
        right.z = forward.x*up.y - forward.y*up.x;
        
        float rl = std::sqrt(right.x*right.x + right.y*right.y + right.z*right.z);
        if (rl > EPS && std::fabs(rl - 1.0f) > EPS) {
            right.x /= rl;
            right.y /= rl;
            right.z /= rl;
        }

        // step 3
        up.x = right.y*forward.z - right.z*forward.y;
        up.y = right.z*forward.x - right.x*forward.z;
        up.z = right.x*forward.y - right.y*forward.x;
    }

    void rotatePitch(float angle) {
        const float EPS = 1e-6f;
        float sinA = std::sin(angle);
        float cosA = std::cos(angle);
        float t = 1.0f - cosA;

        // 1 on 1000000000000000000000
        float rl = std::sqrt(right.x*right.x + right.y*right.y + right.z*right.z);
        if (rl > EPS && std::fabs(rl - 1.0f) > EPS) {
            right.x /= rl;
            right.y /= rl;
            right.z /= rl;
        }

        // create Rodrigues matrix
        std::array<float,16> m = {
            t*right.x*right.x + cosA,      t*right.x*right.y - sinA*right.z, t*right.x*right.z + sinA*right.y, 0.0f,
            t*right.x*right.y + sinA*right.z, t*right.y*right.y + cosA,      t*right.y*right.z - sinA*right.x, 0.0f,
            t*right.x*right.z - sinA*right.y, t*right.y*right.z + sinA*right.x, t*right.z*right.z + cosA,      0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        forward.transformV(m);
        up.transformV(m);

        // orthonormalization aka Gram–Schmidt
        // step 1
        // 'fabs' check absolute eq
        float fl = std::sqrt(forward.x*forward.x + forward.y*forward.y + forward.z*forward.z);
        if (fl > EPS && std::fabs(fl - 1.0f) > EPS) {
            forward.x /= fl;
            forward.y /= fl;
            forward.z /= fl;
        }

        // step 2
        up.x = right.y*forward.z - right.z*forward.y;
        up.y = right.z*forward.x - right.x*forward.z;
        up.z = right.x*forward.y - right.y*forward.x;
        
        float ul = std::sqrt(up.x*up.x + up.y*up.y + up.z*up.z);
        if (ul > EPS && std::fabs(ul - 1.0f) > EPS) {
            up.x /= ul;
            up.y /= ul;
            up.z /= ul;
        }

        // step 3
        right.x = up.y*forward.z - up.z*forward.y;
        right.y = up.z*forward.x - up.x*forward.z;
        right.z = up.x*forward.y - up.y*forward.x;
    }
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

    void projectMeshes() {
        projectedPoints.clear();

        for (auto &mesh : meshes) {
            auto &verts = mesh.getVertices();
            auto &edges = mesh.getEdges();

            std::vector<std::array<float,2>> verts2D(verts.size());

            for (size_t i = 0; i < verts.size(); ++i) {
                const auto &v = verts[i];

                float vx = v.x - camera.position.x;
                float vy = v.y - camera.position.y;
                float vz = v.z - camera.position.z;

                float rdx = vx * camera.right.x   + vy * camera.right.y   + vz * camera.right.z;
                float ry  = vx * camera.up.x      + vy * camera.up.y      + vz * camera.up.z;
                float rdz = vx * camera.forward.x + vy * camera.forward.y + vz * camera.forward.z;

                if (rdz <= 0.0001f) {
                    verts2D[i] = { NAN, NAN };
                    continue;
                }

                float sx = (rdx / rdz) * focal + (W * 0.5f);
                float syi = (ry  / rdz) * focal + (H * 0.5f);
                verts2D[i] = { sx, syi };
            }

            for (const auto &e : edges) {
                int a = e.f1;
                int b = e.f2;
                if (a < 0 || b < 0 || (size_t)a >= verts2D.size() || (size_t)b >= verts2D.size())
                    continue;

                auto &p1 = verts2D[a];
                auto &p2 = verts2D[b];

                if (std::isnan(p1[0]) || std::isnan(p2[0])) continue;

                int x0 = static_cast<int>(p1[0]);
                int y0 = static_cast<int>(p1[1]);
                int x1 = static_cast<int>(p2[0]);
                int y1 = static_cast<int>(p2[1]);

                int dx = std::abs(x1 - x0);
                int dy = std::abs(y1 - y0);
                int sx = x0 < x1 ? 1 : -1;
                int sy = y0 < y1 ? 1 : -1;
                int err = dx - dy;

                while (true) {
                    projectedPoints.push_back({ float(x0), float(y0) });
                    if (x0 == x1 && y0 == y1) break;
                    int e2 = 2 * err;
                    if (e2 > -dy) { err -= dy; x0 += sx; }
                    if (e2 < dx)  { err += dx; y0 += sy; }
                }
            }
        }
    }

    void addMesh(const Mesh& mesh) {
        meshes.push_back(mesh);
    }

    const std::vector<std::array<float,2>>& getProjected() const {
        return projectedPoints;
    }
};