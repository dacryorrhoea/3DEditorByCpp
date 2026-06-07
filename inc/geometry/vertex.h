#pragma once

struct Vertex {
    float x;
    float y;
    float z;

    constexpr Vertex(
        float nx = 0.0f,
        float ny = 0.0f,
        float nz = 0.0f
    ) noexcept : x(nx), y(ny), z(nz) {}

    constexpr void setVertex(float nx, float ny, float nz) noexcept {
        x = nx;
        y = ny;
        z = nz;
    }

    void transformVertex(const std::array<float, 16>& m) noexcept {
        float nx = m[0]*x + m[1]*y + m[2]*z + m[3];
        float ny = m[4]*x + m[5]*y + m[6]*z + m[7];
        float nz = m[8]*x + m[9]*y + m[10]*z + m[11];

        x = nx;
        y = ny;
        z = nz;
    }

    constexpr Vertex operator*(float s) const noexcept {
        return {x * s, y * s, z * s};
    }

    constexpr Vertex operator+(const Vertex& o) const noexcept {
        return {x + o.x, y + o.y, z + o.z};
    }

    constexpr Vertex operator-(const Vertex& o) const noexcept {
        return {x - o.x, y - o.y, z - o.z};
    }

    constexpr Vertex& operator+=(const Vertex& o) noexcept {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }

    constexpr float dot(const Vertex& o) const noexcept {
        return x * o.x + y * o.y + z * o.z;
    }

    void normalized() noexcept {
        float l = std::sqrt(x*x + y*y + z*z);
        x /= l;
        y /= l;
        z /= l;
    }

    constexpr Vertex cross(const Vertex& o) const noexcept {
        return {
            y * o.z - z * o.y,
            z * o.x - x * o.z,
            x * o.y - y * o.x
        };
    }

    friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
        os << v.x << ' ' << v.y << ' ' << v.z;
        return os;
    }
};
