#pragma once

#include <vector>
#include <string>
#include <array>
#include "vertex.h"

struct Faces {
  int f1, f2;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<Faces> faces;
    Vector mesh_center;
    std::string mesh_filepath;
public:
    Mesh() {};
    void loadMeshFromFile(std::string& filepath) {
        // load
    }
    void shiftMesh() {
        // shift
    }
    void rotateMesh() {
        // rotate
    }
};
