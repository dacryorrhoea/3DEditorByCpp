#pragma once

#include <vector>
#include <string>
#include "scene/object.h"
#include "geometry/mesh.h"

class Ground : public Object, public Mesh {
private:
    static std::vector<float> makeVertices(float size, int cells) {
        std::vector<float> vertices;
        const float step = (size * 2.0f) / cells;

        for (int z = 0; z <= cells; ++z) {
            for (int x = 0; x <= cells; ++x) {
                float px = -size + x * step;
                float py = 0.0f;
                float pz = -size + z * step;

                vertices.push_back(px);
                vertices.push_back(py);
                vertices.push_back(pz);
            }
        }

        return vertices;
    }

    static std::vector<int> makeFaces(int cells) {
        std::vector<int> faces;
        const int rowSize = cells + 1;

        for (int z = 0; z < cells; ++z) {
            for (int x = 0; x < cells; ++x) {
                int i0 = z * rowSize + x;
                int i1 = i0 + 1;
                int i2 = i0 + rowSize;
                int i3 = i2 + 1;

                faces.push_back(i0);
                faces.push_back(i2);
                faces.push_back(i1);

                faces.push_back(i1);
                faces.push_back(i2);
                faces.push_back(i3);
            }
        }

        return faces;
    }

public:
    Ground(float size = 500.0f, int cells = 120)
        : Mesh(makeVertices(size, cells), makeFaces(cells))
    {}

    std::string getName() const override { return "Ground"; }
};