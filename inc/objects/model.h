#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include "object.h"
#include "geometry/mesh.h"

class Model : public Object, public Mesh {
private:
    std::string filepath;

    struct FileData {
        std::vector<float> verts;
        std::vector<int> faces;
    };

    static FileData loadFromObj(const std::string& fpath);
    static FileData loadFromStl(const std::string& fpath);
    static FileData loadByExtension(const std::string& fpath);

    Model(FileData data, std::string path);

public:
    explicit Model(const Mesh& mesh);
    explicit Model(Mesh&& mesh);
    explicit Model(const std::string& fpath);

    void saveObjectToObjFile(Mesh* mesh, std::string fname = "Model");
};