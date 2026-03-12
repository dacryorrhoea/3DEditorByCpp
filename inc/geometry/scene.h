#pragma once

#include <cmath>
#include <array>
#include <vector>
#include "mesh.h"
#include "vertex.h"


class Scene {
private:
    int W;
    int H;
    Vertex camera;
    float focal;
    float pitch;
    float yaw;
    std::vector<Mesh> meshes;
    std::vector<std::array<float,2>> projectedPoints;

public:
    Scene(int w, int h)
        : W(w)
        , H(h)
        , camera(0.0f, 2.0f, -5.0f)
        , focal( (W * 0.5f) / std::tan( (60.0f * 3.14159265358979323846f / 180.0f) * 0.5f ) )
        , pitch(0.0f)
        , yaw(0.0f)
    {}

    void projectMeshes() {
        projectedPoints.clear();

        for (auto &mesh : meshes) {
            auto &verts = mesh.getVertices();
            auto &edges = mesh.getEdges();

            // предварительно проецируем все вершины
            std::vector<std::array<float,2>> verts2D(verts.size());
            for (size_t i = 0; i < verts.size(); ++i) {
                const auto &v = verts[i];
                float dx = v.v[0] - camera.v[0];
                float dy = v.v[1] - camera.v[1];
                float dz = v.v[2] - camera.v[2];

                float cp = std::cos(pitch);
                float sp = std::sin(pitch);
                float ry = cp * dy - sp * dz;
                float rz = sp * dy + cp * dz;
                float rx = dx;

                float cy = std::cos(yaw);
                float sy = std::sin(yaw);
                float rdx =  cy * rx + sy * rz;
                float rdz = -sy * rx + cy * rz;

                if (rdz <= 0.0001f) {
                    verts2D[i] = { NAN, NAN };
                    continue;
                }

                float sx = (rdx / rdz) * focal + (W * 0.5f);
                float syi = (ry  / rdz) * focal + (H * 0.5f);
                verts2D[i] = { sx, syi };
            }

            // для каждого ребра строим точки вдоль линии
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