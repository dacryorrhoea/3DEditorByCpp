#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <cmath>
#include "vertex.h"
#include "faces.h"



class Mesh {
protected:
    std::vector<Vertex> vertices;
    std::vector<Faces> faces;

    void shiftX(float shift) {
        transformVertexSet({
            1.0f, 0.0f, 0.0f, shift,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });
    }

    void shiftY(float shift) {
        transformVertexSet({
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, shift,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });
    }

    void shiftZ(float shift) {
        transformVertexSet({
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, shift,
            0.0f, 0.0f, 0.0f, 1.0f
        });
    }

    void rotateX(float angle, float cx = 0.0f, float cy = 0.0f) {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        transformVertexSet({
            cosA, -sinA, 0.0f, (1.0f - cosA) * cx + sinA * cy,
            sinA,  cosA, 0.0f, -sinA * cx + (1.0f - cosA) * cy,
            0.0f,  0.0f, 1.0f, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f
        });
    }

    void rotateY(float angle, float cx = 0.0f, float cz = 0.0f) {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        transformVertexSet({
            cosA,  0.0f, sinA, (1.0f - cosA) * cx - sinA * cz,
            0.0f,  1.0f, 0.0f, 0.0f,
            -sinA, 0.0f, cosA, sinA * cx + (1.0f - cosA) * cz,
            0.0f,  0.0f, 0.0f, 1.0f
        });
    }

    void rotateZ(float angle, float cx = 0.0f, float cy = 0.0f) {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        transformVertexSet({
            cosA, -sinA, 0.0f, (1.0f - cosA) * cx + sinA * cy,
            sinA,  cosA, 0.0f, -sinA * cx + (1.0f - cosA) * cy,
            0.0f,  0.0f, 1.0f, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f
        });
    }

    void transformVertexSet(const std::array<float, 16>& m) noexcept {
        for (auto& v : vertices) {
            v.transformVertex(m);
        }
    }

    void transformVertexSubset(
        const std::array<float, 16>& m,
        const int begin,
        const int end
    ) noexcept {
        //
    }

public:
    Mesh(const std::vector<float>& v, const std::vector<int>& f) {
        if (f.size() % 3 != 0 || v.size() % 3 != 0)
            throw std::invalid_argument("must be multiples of 3");

        vertices.reserve(v.size() / 3);
        for (size_t i = 0; i < v.size(); i += 3) {
            vertices.emplace_back(v[i], v[i + 1], v[i + 2]);
        }

        faces.reserve(f.size() / 3);
        for (size_t i = 0; i < f.size(); i += 3)
            faces.push_back({f[i], f[i+1], f[i+2]});
    }

    void getTransformedVertices(
        const Vertex& position,
        const Vertex& forward,
        const Vertex& right,
        const Vertex& up,
        std::vector<Vertex>& out
    ) const
    {
        out.clear();
        out.reserve(vertices.size());

        for (const Vertex& v : vertices)
        {
            out.emplace_back(
                position + right * v.x + up * v.y + forward * v.z
            );
        }
    }

    std::vector<Faces>& getFaces() { return faces; }
};
