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
public:
    // конструктор наследуется от vertexset и добавляет запись полученныых faces
    Mesh(
        const std::vector<float>& v,
        const std::vector<int>& f
    ) : VertexSet(v)  {
        if (f.size() % 3 != 0)
            throw std::invalid_argument("must be multiples of 3");
        faces.reserve(f.size() / 3);
        for (size_t i = 0; i < f.size(); i += 3)
            faces.push_back({f[i], f[i+1], f[i+2]});
    }

    std::vector<Faces>& getFaces() { return faces; }

    void shiftMesh() {
        // poof
    }

    void rotateMesh(float angle) {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        float cx = 0.5f;
        float cy = 0.5f;
        float cz = 0.5f;

        std::array<float,16> mX = {
            cosA, -sinA, 0.0f, (1.0f - cosA) * cx + sinA * cy,
            sinA,  cosA, 0.0f, -sinA * cx + (1.0f - cosA) * cy,
            0.0f,  0.0f, 1.0f, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f
        };

        std::array<float,16> mY = {
            cosA,  0.0f, sinA, (1.0f - cosA) * cx - sinA * cz,
            0.0f,  1.0f, 0.0f, 0.0f,
            -sinA, 0.0f, cosA, sinA * cx + (1.0f - cosA) * cz,
            0.0f,  0.0f, 0.0f, 1.0f
        };

        std::array<float,16> mZ = {
            cosA, -sinA, 0.0f, (1.0f - cosA) * cx + sinA * cy,
            sinA,  cosA, 0.0f, -sinA * cx + (1.0f - cosA) * cy,
            0.0f,  0.0f, 1.0f, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f
        };

        transformVertexSet(mY);
    }
};
