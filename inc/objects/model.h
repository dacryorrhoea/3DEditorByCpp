#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include "scene/object.h"
#include "geometry/mesh.h"

class Model : public Object, public Mesh {
private:
    std::string filepath;

    struct FileData {
        std::vector<float> verts;
        std::vector<int> faces;
    };

    static FileData loadFromObj(const std::string& fpath) {
        std::ifstream file(fpath);
        if (!file.is_open()) throw std::runtime_error("Cannot open file");

        FileData data;
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (line[0] == '#') continue;

            std::stringstream ss(line);
            std::string prefix;
            if (!(ss >> prefix)) continue;

            if (prefix == "v") {
                float x, y, z;
                if (!(ss >> x >> y >> z)) {
                    throw std::runtime_error("Bad vertex line: " + line);
                }
                data.verts.insert(data.verts.end(), { x, y, z });
            }
            else if (prefix == "f") {
                std::vector<int> idx;
                std::string token;

                const int vertexCount = static_cast<int>(
                    data.verts.size() / 3
                );

                while (ss >> token) {
                    if (token.empty()) continue;

                    size_t p = token.find('/');
                    std::string vstr = (p == std::string::npos)
                        ? token
                        : token.substr(0, p);

                    if (vstr.empty()) continue;

                    int vi;
                    try {
                        vi = std::stoi(vstr);
                    }
                    catch (...) {
                        throw std::runtime_error(
                            "Bad face token: " + token
                        );
                    }

                    if (vi < 0) {
                        vi = vertexCount + vi;
                    } else {
                        vi = vi - 1;
                    }

                    if (vi < 0 || vi >= vertexCount) {
                        throw std::runtime_error("Face index out of range: " + token);
                    }

                    idx.push_back(vi);
                }

                if (idx.size() < 3) continue;

                for (size_t k = 1; k + 1 < idx.size(); ++k) {
                    data.faces.insert(data.faces.end(), { idx[0], idx[k], idx[k + 1] });
                }
            }
        }

        return data;
    }

    static FileData loadFromStl(const std::string& fpath) {
        FileData data;
        // @_@
        return data;
    }

    FileData loadByExtension(const std::string& fpath) {
        std::filesystem::path p(fpath);
        std::string ext = p.extension().string();

        filepath = fpath;        
        if (ext == ".obj") return loadFromObj(fpath);
        if (ext == ".stl") return loadFromStl(fpath);

        throw std::runtime_error("Unsup: " + ext);
    }

    explicit Model(FileData data)
        : Mesh(data.verts, data.faces)
    {}
public:
    explicit Model(const Mesh& mesh)
        : Object(), Mesh(mesh)
    {
        object_name = "CustomModel";
        saveble = true;
        color = 0xFF69B4FF;
    }

    explicit Model(Mesh&& mesh)
        : Object(), Mesh(std::move(mesh))
    {
        object_name = "Model";
        saveble = true;
        color = 0xFF69B4FF;
    }

    explicit Model(const std::string& fpath)
        : Model(loadByExtension(fpath))
    {
        object_name = "Model";
        saveble = true;
        color = 0xFF69B4FF;
    }

    void saveObjectToObjFile(Mesh* mesh, std::string fname = "Model") {
        std::string path = "./data/" + fname + ".obj";
        std::ofstream out(path);
        if (!out.is_open()) throw std::runtime_error("Cannot open file");

        out.imbue(std::locale::classic()); // точка в дробях, а не запятая

        out << "# Wavefront OBJ file\n";
        out << "o " << fname << '\n';
        out << "s off\n";

        const VertexContainer& verts = mesh->getVertecies();
        const FaceContainer& faces = mesh->getFaces();

        out << std::setprecision(std::numeric_limits<float>::max_digits10);

        for (auto vert : verts) {
            out << "v " << vert << '\n';
        }

        if (faces.Size() % 3 != 0) {
            throw std::runtime_error("multiple of 3 indices");
        }

        for (auto face : faces) {
            out << "f " << face + 1 << '\n';
        }
    }
};