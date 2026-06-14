#pragma once

#include <iostream>
#include <exception>
#include <vector>
#include <cmath>
#include "object.h"
#include "geometry/mesh.h"

class Sphere : public Object, public Mesh {
private:
    static std::vector<float> buildVertices(int stacks, int slices) {
        std::vector<float> vertices;
        const float pi = 3.1415926535f;

        for (int i = 0; i <= stacks; ++i) {
            float v = static_cast<float>(i) / stacks;
            float phi = pi * v;

            float y = std::cos(phi);
            float r = std::sin(phi);

            for (int j = 0; j <= slices; ++j) {
                float u = static_cast<float>(j) / slices;
                float theta = 2.0f * pi * u;

                float x = r * std::cos(theta);
                float z = r * std::sin(theta);

                vertices.push_back(x * 0.5f);
                vertices.push_back(y * 0.5f);
                vertices.push_back(z * 0.5f);
            }
        }

        return vertices;
    }

    static std::vector<int> buildIndices(int stacks, int slices) {
        std::vector<int> indices;

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                int first  = i * (slices + 1) + j;
                int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(first + 1);
                indices.push_back(second);
                indices.push_back(second + 1);
            }
        }

        return indices;
    }

public:
    Sphere(int stacks = 16, int slices = 24)
        : Mesh(buildVertices(stacks, slices), buildIndices(stacks, slices))
    {
        try {
            object_name = "Sphere";
            color = 0xFFFF22FF;
        } catch (const std::exception& e) {
            std::cerr << "Exception in Sphere constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Sphere constructor"
                      << std::endl;
            throw;
        }
    }
};