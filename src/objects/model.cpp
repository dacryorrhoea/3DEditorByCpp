#include "suph/all_objects.h"
#include "suph/pch.h"

Model::FileData Model::loadFromObj(const std::string& fpath) {
    std::ifstream file(fpath);
    if (!file.is_open()) throw std::runtime_error(
        "Cannot open file by path: " + fpath
    );

    FileData data;
    std::string line;

    // индекс строки нужен для указания в ошибках
    for (int str_i = 1; std::getline(file, line); str_i++)
    {
        // извлечение первого слова в строке
        // если отдельного слова нет то следующая строка
        std::stringstream ss(line);
        std::string prefix;
        if (!(ss >> prefix)) continue;
        
        // строки с вершинами начинаются с символа v
        // строки с гранями начинаются с символа f
        // иные строки пропускаются 
        if (prefix == "v")
        {
            float x, y, z;
            if (!(ss >> x >> y >> z) || (ss >> std::ws).peek() != EOF)
            {
                throw std::runtime_error(
                    "Bad vertex in line " +
                    std::to_string(str_i) +
                    ": " + line
                );
            } 
            data.verts.insert(data.verts.end(), {x, y, z});
        }
        else if (prefix == "f")
        {
            std::vector<int> idx;
            int vertexCount = static_cast<int>(
                data.verts.size() / 3
            );

            std::string token;
            while (ss >> token)
            {
                size_t p = token.find('/');
                std::string vstr = (p == std::string::npos)
                    ? token
                    : token.substr(0, p);

                int vi;
                try
                {
                    vi = std::stoi(vstr);
                }
                catch (...)
                {
                    throw std::runtime_error(
                        "Bad face token in line " +
                        std::to_string(str_i) +
                        ": " + token
                    );
                }

                if (vi < 0) vi = vertexCount + vi;
                else vi = vi - 1;

                if (vi < 0 || vi >= vertexCount)
                {
                    throw std::runtime_error(
                        "Face index out of range in line " +
                        std::to_string(str_i) +
                        ": " + token
                    );
                }

                idx.push_back(vi);
            }

            if (idx.size() < 3) continue;

            for (size_t k = 1; k + 1 < idx.size(); ++k) {
                data.faces.insert(
                    data.faces.end(),
                    { idx[0], idx[k], idx[k + 1] }
                );
            }
        }
    }

    return data;
}

Model::FileData Model::loadFromStl(const std::string& fpath) {
    auto isBinaryStl = [](const std::string& path) -> bool {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open file by path: " + path
            );
        }

        std::streamsize fileSize = file.tellg();
        if (fileSize < 84) return false;

        file.seekg(80, std::ios::beg);

        std::uint32_t triCount;
        file.read(reinterpret_cast<char*>(&triCount), sizeof(triCount));
        if (!file) return false;

        return fileSize == 84
            + static_cast<std::streamsize>(triCount)
            * 50;
    };

    FileData data;

    if (isBinaryStl(fpath))
    {
        std::ifstream file(fpath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open file by path: " + fpath
            );
        }

        char header[80];
        file.read(header, 80);

        std::uint32_t triCount;
        file.read(reinterpret_cast<char*>(&triCount), sizeof(triCount));

        for (std::uint32_t i = 0; i < triCount; i++)
        {
            float normal[3];
            float verts[3][3];
            std::uint16_t attr;

            file.read(reinterpret_cast<char*>(normal), sizeof(normal));
            file.read(reinterpret_cast<char*>(verts), sizeof(verts));
            file.read(reinterpret_cast<char*>(&attr), sizeof(attr));

            if (!file) {
                throw std::runtime_error(
                    "Bad STL triangle #" + std::to_string(i)
                );
            }

            int base = static_cast<int>(data.verts.size() / 3);

            for (int v = 0; v < 3; v++) {
                data.verts.insert(data.verts.end(), {
                    verts[v][0],
                    verts[v][1],
                    verts[v][2]
                });
            }

            data.faces.insert(
                data.faces.end(),
                { base, base + 1, base + 2 }
            );
        }
    }
    else
    {
        std::ifstream file(fpath);
        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open file by path: " + fpath
            );
        }

        std::string line;
        int vertexInFacet = 0;
        int base = 0;

        for (int str_i = 1; std::getline(file, line); str_i++)
        {
            std::stringstream ss(line);

            std::string prefix;
            if (!(ss >> prefix)) continue;

            if (prefix == "vertex")
            {
                float x, y, z;

                if (!(ss >> x >> y >> z) ||
                    (ss >> std::ws).peek() != EOF)
                {
                    throw std::runtime_error(
                        "Bad vertex in line " +
                        std::to_string(str_i) +
                        ": " + line
                    );
                }

                if (vertexInFacet == 0) {
                    base = static_cast<int>(
                        data.verts.size() / 3
                    );
                }

                data.verts.insert(
                    data.verts.end(),
                    { x, y, z }
                );

                vertexInFacet++;

                if (vertexInFacet == 3)
                {
                    data.faces.insert(
                        data.faces.end(),
                        { base, base + 1, base + 2 }
                    );

                    vertexInFacet = 0;
                }
            }
        }
    }

    return data;
}

Model::FileData Model::loadByExtension(const std::string& fpath) {
    std::filesystem::path p(fpath);
    std::string ext = p.extension().string();

    if (ext == ".obj") return loadFromObj(fpath);
    if (ext == ".stl") return loadFromStl(fpath);

    throw std::runtime_error("Unsup: " + ext);
}

Model::Model(FileData data, std::string path)
    : Mesh(data.verts, data.faces),
      filepath(std::move(path))
{}

Model::Model(const Mesh& mesh)
    : Object(), Mesh(mesh)
{
    object_name = "CustomModel";
    saveble = true;
    color = 0xFF69B4FF;
}

Model::Model(Mesh&& mesh)
    : Object(), Mesh(std::move(mesh))
{
    object_name = "Model";
    saveble = true;
    color = 0xFF69B4FF;
}

Model::Model(const std::string& fpath)
    : Model(loadByExtension(fpath), fpath)
{
    object_name = "Model";
    saveble = true;
    color = 0xFF69B4FF;
}

void Model::saveObjectToObjFile(Mesh* mesh, std::string fname) {
    std::string path = "./data/" + fname + ".obj";
    std::ofstream out(path);
    if (!out.is_open()) throw std::runtime_error("Cannot open file");

    out.imbue(std::locale::classic());

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