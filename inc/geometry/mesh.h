#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <cmath>
#include "vertex.h"

struct Edge {
  int f1, f2;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    Vertex mesh_center;
    std::string mesh_filepath;
public:
    Mesh() {};

    std::vector<Vertex>& getVertices() { return vertices; }
    std::vector<Edge>& getEdges() { return edges; }

    void loadMeshFromFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) throw std::runtime_error("Cannot open file");

        std::string line;

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v") {
                float x, y, z;
                ss >> x >> y >> z;
                vertices.emplace_back(x, y, z);
            }
            else if (prefix == "l") {
                int i1, i2;
                ss >> i1 >> i2;

                edges.push_back({ i1 - 1, i2 - 1 });
            }
        }
    }

    // void printDebugInfo() const {
    //     for (size_t i = 0; i < vertices.size(); ++i) {
    //         const auto &v = vertices[i].v;
    //         std::cout << "[" << i << "] " << v[0] << ", " << v[1] << ", " << v[2] << "\n";
    //     }
    //     for (size_t i = 0; i < edges.size(); ++i) {
    //         const auto &e = edges[i];
    //         std::cout << "[" << i << "] " << e.f1 << " - " << e.f2 << "\n";
    //     }
    // }
    
    void shiftMesh() {
        // shift
    }

    void rotateMesh(float angle) {
        const Vertex mesh_center = {0.5f, 0.5f, 0.5f};
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        float cx = mesh_center.x;
        float cy = mesh_center.y;
        float cz = mesh_center.z;

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

        for (auto& vert : vertices) {
            vert.transformV(mY);
        }
    }
};
