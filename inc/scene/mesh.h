#pragma once

#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
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

    void printDebugInfo() const {
        // вершины
        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto &v = vertices[i].v;
            std::cout << "[" << i << "] "
                    << v[0] << ", " << v[1] << ", " << v[2] << "\n";
        }
        // рёбра
        for (size_t i = 0; i < edges.size(); ++i) {
            const auto &e = edges[i];
            std::cout << "[" << i << "] " << e.f1 << " - " << e.f2 << "\n";
        }
    }
    
    void shiftMesh() {
        // shift
    }

    void rotateMesh() {
        // rotate
    }
};
