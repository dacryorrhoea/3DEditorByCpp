#include "pch.h"

#include "scene.h"
#include "geometry/vertex.h"
#include "custom_types.h"

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

    forward.transformVertex(m);

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

    forward.transformVertex(m);

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

void Scene::toProjectingScene() {
    polygons.clear();

    const float HW = W * 0.5f;
    const float HH = H * 0.5f;
    
    for (auto& meshPtr : meshes) {
        Mesh& mesh = *meshPtr;
        std::vector<Vertex>& vertices = mesh.getVertices();
        std::vector<Faces>& faces = mesh.getFaces();
        std::vector<Vertex> verticesCamSpace(vertices.size());

        for (size_t i = 0; i < vertices.size(); ++i) {
            float vx = vertices[i].x - camera.position.x;
            float vy = vertices[i].y - camera.position.y;
            float vz = vertices[i].z - camera.position.z;

            verticesCamSpace[i].x = vx * camera.right.x   + vy * camera.right.y   + vz * camera.right.z;
            verticesCamSpace[i].y = vx * camera.up.x      + vy * camera.up.y      + vz * camera.up.z;
            verticesCamSpace[i].z = vx * camera.forward.x + vy * camera.forward.y + vz * camera.forward.z;
        }

        for (Faces& face : faces) {
            Vertex v1 = verticesCamSpace[face.f1];
            Vertex v2 = verticesCamSpace[face.f2];
            Vertex v3 = verticesCamSpace[face.f3];

            if (std::abs(v1.z) < 0.01f || std::abs(v2.z) < 0.01f || std::abs(v3.z) < 0.01f) continue;

            Vertex u1, u2;
            u1.x = v2.x - v1.x;
            u1.y = v2.y - v1.y;
            u1.z = v2.z - v1.z;

            u2.x = v3.x - v1.x;
            u2.y = v3.y - v1.y;
            u2.z = v3.z - v1.z;

            Vertex n;
            n.x = u1.y * u2.z - u1.z * u2.y;
            n.y = u1.z * u2.x - u1.x * u2.z;
            n.z = u1.x * u2.y - u1.y * u2.x;

            if (std::abs(n.x) + std::abs(n.y) + std::abs(n.z) < 1e-6) continue;

            float dot = n.x * (v1.x + v2.x + v3.x) / 3 +
                        n.y * (v1.y + v2.y + v3.y) / 3 +
                        n.z * (v1.z + v2.z + v3.z) / 3;

            if (dot < 0) continue;

            Polygon pol;
            pol.p1.x = static_cast<int>((v1.x / v1.z) * focal + HW + 0.5f);
            pol.p1.y = static_cast<int>(-(v1.y / v1.z) * focal + HH + 0.5f);
            pol.p1.inv_z = 1/v1.z;

            pol.p2.x = static_cast<int>((v2.x / v2.z) * focal + HW + 0.5f);
            pol.p2.y = static_cast<int>(-(v2.y / v2.z) * focal + HH + 0.5f);
            pol.p2.inv_z = 1/v2.z;

            pol.p3.x = static_cast<int>((v3.x / v3.z) * focal + HW + 0.5f);
            pol.p3.y = static_cast<int>(-(v3.y / v3.z) * focal + HH + 0.5f);
            pol.p3.inv_z = 1/v3.z;

            pol.color = 0xFF69B4FF;

            polygons.push_back(pol);
        }
    }
}