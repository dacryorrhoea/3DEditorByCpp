#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <cmath>
#include "vertex.h"
#include "vertex_set.h"

struct Faces {
    int f1, f2, f3;
};


class Mesh : public VertexSet {
protected:
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
public:
    Mesh(const std::vector<float>& v, const std::vector<int>& f) : VertexSet(v)  {
        if (f.size() % 3 != 0) throw std::invalid_argument("must be multiples of 3");
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
