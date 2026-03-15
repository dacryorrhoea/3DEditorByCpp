#pragma once

struct Vertex {
    float x, y, z;

    constexpr Vertex(
        float nx = 0.0f,
        float ny = 0.0f,
        float nz = 0.0f
    ) noexcept : x(nx), y(ny), z(nz) {}

    constexpr void setV(float nx, float ny, float nz) noexcept {
        x = nx;
        y = ny;
        z = nz;
    }

    // constexpr void translateV(float t) noexcept {
    //     x += t;
    //     y += t;
    //     z += t;
    // }

    // constexpr void scaleV(float t) noexcept {
    //     x *= t;
    //     y *= t;
    //     z *= t;
    // }

    void transformV(const std::array<float, 16>& m) noexcept {
        float nx = m[0]*x + m[1]*y + m[2]*z + m[3];
        float ny = m[4]*x + m[5]*y + m[6]*z + m[7];
        float nz = m[8]*x + m[9]*y + m[10]*z + m[11];

        x = nx;
        y = ny;
        z = nz;
    }
};
