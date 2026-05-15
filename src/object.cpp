#include "sup_class/pch.h"

#include "scene/object.h"

void Object::rotateYaw(float angle) {
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

    // orthonormalization aka Gram–Schmidt
    forward.transformVertex(m);
    forward.normalized();

    right = forward.cross({0.0f, 1.0f, 0.0f});
    right.normalized();

    up = right.cross(forward);
    up.normalized();

    changed = true;
};

void Object::rotatePitch(float angle) {
    float sinA = std::sin(angle);
    float cosA = std::cos(angle);
    float t = 1.0f - cosA;

    // create Rodrigues matrix
    std::array<float,16> m = {
        t*right.x*right.x + cosA,         t*right.x*right.y - sinA*right.z, t*right.x*right.z + sinA*right.y, 0.0f,
        t*right.x*right.y + sinA*right.z, t*right.y*right.y + cosA,         t*right.y*right.z - sinA*right.x, 0.0f,
        t*right.x*right.z - sinA*right.y, t*right.y*right.z + sinA*right.x, t*right.z*right.z + cosA,         0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // orthonormalization aka Gram–Schmidt
    forward.transformVertex(m);
    forward.normalized();

    up = right.cross(forward);
    up.normalized();

    right = forward.cross(up);
    right.normalized();

    changed = true;
};