#pragma once

#include <filesystem>
#include "scene/object.h"
#include "geometry/mesh.h"

class Model : public Object, public Mesh {
private:
    struct FileData {
        std::vector<float> verts;
        std::vector<int> faces;
    };

    static FileData loadFromObj(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) throw std::runtime_error("Cannot open file");

        FileData data;
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;
            if (prefix == "v") {
                float x, y, z;
                ss >> x >> y >> z;
                data.verts.insert(data.verts.end(), {x, y, z});
            } else if (prefix == "f") {
                std::string token;
                std::vector<int> idx;
                while (ss >> token) {
                    size_t p = token.find('/');
                    std::string vstr = (p == std::string::npos) ? token : token.substr(0, p);
                    if (vstr.empty()) continue;
                    int vi = std::stoi(vstr);
                    vi = (vi < 0) ? static_cast<int>(data.verts.size() / 3) + vi : vi - 1;
                    idx.push_back(vi);
                }
                for (size_t k = 1; k + 1 < idx.size(); ++k)
                    data.faces.insert(data.faces.end(), {idx[0], idx[k], idx[k+1]});
            }
        }
        return data;
    }

    static FileData loadFromStl(const std::string& filepath) {
        FileData data;
        // @_@
        return data;
    }

    static FileData loadByExtension(const std::string& filepath) {
        std::filesystem::path p(filepath);
        std::string ext = p.extension().string();

        if (ext == ".obj") return loadFromObj(filepath);
        if (ext == ".stl") return loadFromStl(filepath);

        throw std::runtime_error("Unsup: " + ext);
    }

    explicit Model(FileData data) : Mesh(data.verts, data.faces) {}
public:
    explicit Model(const std::string& filepath) : Model(loadByExtension(filepath)) {
        color = 0xFF69B4FF;
    }

    std::string getName() const override { return "Model"; }
};