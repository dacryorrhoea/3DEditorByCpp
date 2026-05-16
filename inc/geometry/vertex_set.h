#pragma once

#include <vector>
#include "vertex.h"

class VertexSet {
protected:
    std::vector<Vertex> vertices;
public:
    explicit VertexSet(const std::vector<float>& c) {
        if (c.size() % 3 != 0) {
            throw std::invalid_argument("must be multiples of 3");
        }
        vertices.reserve(c.size() / 3);
        for (size_t i = 0; i < c.size(); i += 3) {
            vertices.emplace_back(c[i], c[i + 1], c[i + 2]);
        }
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
};