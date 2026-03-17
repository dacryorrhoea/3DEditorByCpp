#include "pch.h"

#include "scene.h"
#include "geometry/vertex.h"

void Camera::rotateYaw(float angle) {
    float sinA = std::sin(angle);
    float cosA = std::cos(angle);
    float t = 1.0f - cosA;

    // create Rodrigues matrix
    std::array<float,16> m = {
        cosA,  0.0f,     sinA, 0.0f,
        0.0f,  t + cosA, 0.0f, 0.0f,
        -sinA, 0.0f,     cosA, 0.0f,
        0.0f,  0.0f,     0.0f, 1.0f
    };

    forward.transformV(m);

    // orthonormalization aka Gram–Schmidt
    // step 1
    // 'fabs' check absolute eq
    float fl = std::sqrt(forward.x*forward.x + forward.y*forward.y + forward.z*forward.z);
    forward.x /= fl;
    forward.y /= fl;
    forward.z /= fl;

    // step 2
    right.x = forward.y*0.0f - forward.z*1.0f;
    right.y = forward.z*0.0f - forward.x*0.0f;
    right.z = forward.x*1.0f - forward.y*0.0f;
    
    float rl = std::sqrt(right.x*right.x + right.y*right.y + right.z*right.z);
    right.x /= rl;
    right.y /= rl;
    right.z /= rl;

    // step 3
    up.x = right.y*forward.z - right.z*forward.y;
    up.y = right.z*forward.x - right.x*forward.z;
    up.z = right.x*forward.y - right.y*forward.x;

    float ul = std::sqrt(up.x*up.x + up.y*up.y + up.z*up.z);
    up.x /= ul;
    up.y /= ul;
    up.z /= ul;
}

void Camera::rotatePitch(float angle) {
    float sinA = std::sin(angle);
    float cosA = std::cos(angle);
    float t = 1.0f - cosA;

    // create Rodrigues matrix
    std::array<float,16> m = {
        t*right.x*right.x + cosA,      t*right.x*right.y - sinA*right.z, t*right.x*right.z + sinA*right.y, 0.0f,
        t*right.x*right.y + sinA*right.z, t*right.y*right.y + cosA,      t*right.y*right.z - sinA*right.x, 0.0f,
        t*right.x*right.z - sinA*right.y, t*right.y*right.z + sinA*right.x, t*right.z*right.z + cosA,      0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    forward.transformV(m);

    // orthonormalization aka Gram–Schmidt
    // step 1
    // 'fabs' check absolute eq
    float fl = std::sqrt(forward.x*forward.x + forward.y*forward.y + forward.z*forward.z);
    forward.x /= fl;
    forward.y /= fl;
    forward.z /= fl;

    // step 2
    up.x = right.y*forward.z - right.z*forward.y;
    up.y = right.z*forward.x - right.x*forward.z;
    up.z = right.x*forward.y - right.y*forward.x;
    
    float ul = std::sqrt(up.x*up.x + up.y*up.y + up.z*up.z);
    up.x /= ul;
    up.y /= ul;
    up.z /= ul;

    // step 3
    right.x = forward.y*up.z - forward.z*up.y;
    right.y = forward.z*up.x - forward.x*up.z;
    right.z = forward.x*up.y - forward.y*up.x;

    float rl = std::sqrt(right.x*right.x + right.y*right.y + right.z*right.z);
    right.x /= rl;
    right.y /= rl;
    right.z /= rl;
}

void Scene::projectMeshes() {
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