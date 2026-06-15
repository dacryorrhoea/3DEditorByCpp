#include "suph/pch.h"
#include "objects/object.h"

void Object::rotateYaw(float angle) {
    try {
        float sinA = std::sin(angle);
        float cosA = std::cos(angle);
        float t = 1.0f - cosA;

        std::array<float, 16> m = {
            cosA,  0.0f,     sinA, 0.0f,
            0.0f,  t + cosA, 0.0f, 0.0f,
            -sinA, 0.0f,     cosA, 0.0f,
            0.0f,  0.0f,     0.0f, 1.0f
        };

        forward.transformVertex(m);
        forward.normalized();

        right = forward.cross({0.0f, 1.0f, 0.0f});
        right.normalized();

        up = right.cross(forward);
        up.normalized();

        changed = true;
    } catch (const std::exception& e) {
        std::cerr << "Exception in Object::rotateYaw: "
                  << e.what()
                  << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception in Object::rotateYaw"
                  << std::endl;
        throw;
    }
}

void Object::rotatePitch(float angle) {
    try {
        float sinA = std::sin(angle);
        float cosA = std::cos(angle);
        float t = 1.0f - cosA;

        std::array<float, 16> m = {
            t*right.x*right.x + cosA,
            t*right.x*right.y - sinA*right.z,
            t*right.x*right.z + sinA*right.y,
            0.0f,

            t*right.x*right.y + sinA*right.z,
            t*right.y*right.y + cosA,
            t*right.y*right.z - sinA*right.x,
            0.0f,

            t*right.x*right.z - sinA*right.y,
            t*right.y*right.z + sinA*right.x,
            t*right.z*right.z + cosA,
            0.0f,

            0.0f, 0.0f, 0.0f, 1.0f
        };

        forward.transformVertex(m);
        forward.normalized();

        up = right.cross(forward);
        up.normalized();

        right = forward.cross(up);
        right.normalized();

        changed = true;
    } catch (const std::exception& e) {
        std::cerr << "Exception in Object::rotatePitch: "
                  << e.what()
                  << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception in Object::rotatePitch"
                  << std::endl;
        throw;
    }
}