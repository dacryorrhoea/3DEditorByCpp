#pragma once

#include <array>

struct Vertex {
    std::array<float,3> v{};

    constexpr Vertex(
        float x = 0.0f,
        float y = 0.0f,
        float z = 0.0f
    ) : v{x, y, z} {}

    void setV(float x, float y, float z) {
        v = {x, y, z};
    }

    void transformV(const std::array<float, 16>& m) {
        float x0 = v[0], y0 = v[1], z0 = v[2];

        v[0] = m[0]*x0 + m[1]*y0 + m[2]*z0 + m[3];
        v[1] = m[4]*x0 + m[5]*y0 + m[6]*z0 + m[7];
        v[2] = m[8]*x0 + m[9]*y0 + m[10]*z0 + m[11];  
    }
};
