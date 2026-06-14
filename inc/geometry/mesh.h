#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <cmath>
#include <exception>
#include "containers/vertices.h"
#include "containers/faces.h"

class Mesh {
protected:
    VertexContainer vertices;
    FaceContainer faces;

    void transformVertexSubset(
        const std::array<float, 16>& m,
        const std::vector<int>& subv
    ) noexcept {
        for (int v : subv) {
            vertices[v].transformVertex(m);
        }
    }

public:
    Mesh() {}

    Mesh(const Mesh&) = default;

    Mesh(const std::vector<float>& v, const std::vector<int>& f) {
        try {
            if (f.size() % 3 != 0 || v.size() % 3 != 0)
                throw std::invalid_argument("must be multiples of 3");

            vertices.Reserve(v.size() / 3);
            for (size_t i = 0; i < v.size(); i += 3) {
                vertices.Add(Vertex(v[i], v[i + 1], v[i + 2]));
            }

            faces.Reserve(f.size() / 3);
            for (size_t i = 0; i < f.size(); i += 3) {
                faces.Add(Face(f[i], f[i + 1], f[i + 2]));
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in Mesh constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Mesh constructor"
                      << std::endl;
            throw;
        }
    }

    void getTransformedVertices(
        const Vertex& position,
        const Vertex& forward,
        const Vertex& right,
        const Vertex& up,
        VertexContainer& out
    ) const {
        try {
            out.Clear();
            out.Reserve(vertices.Size());

            for (const Vertex& v : vertices) {
                out.Add(
                    position +
                    right * v.x +
                    up * v.y +
                    forward * v.z
                );
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in getTransformedVertices: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in getTransformedVertices"
                      << std::endl;
            throw;
        }
    }

    void setTransformedVertices(
        const Vertex& position,
        const Vertex& forward,
        const Vertex& right,
        const Vertex& up
    ) {
        try {
            VertexContainer new_vert;
            new_vert.Reserve(vertices.Size());

            for (const Vertex& v : vertices) {
                new_vert.Add(
                    position +
                    right * v.x +
                    up * v.y +
                    forward * v.z
                );
            }

            vertices = new_vert;
        } catch (const std::exception& e) {
            std::cerr << "Exception in setTransformedVertices: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in setTransformedVertices"
                      << std::endl;
            throw;
        }
    }

    void shiftVertexSetX(float shift, std::vector<int>& subv) {
        transformVertexSubset(
            {
                1.0f, 0.0f, 0.0f, shift,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            },
            subv
        );
    }

    void shiftVertexSetY(float shift, std::vector<int>& subv) {
        transformVertexSubset(
            {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, shift,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            },
            subv
        );
    }

    void shiftVertexSetZ(float shift, std::vector<int>& subv) {
        transformVertexSubset(
            {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, shift,
                0.0f, 0.0f, 0.0f, 1.0f
            },
            subv
        );
    }

    void scaleMesh(float scale) {
        if (vertices.Size() == 0) return;

        Vertex center(0.0f, 0.0f, 0.0f);
        for (const auto& v : vertices) {
            center.x += v.x;
            center.y += v.y;
            center.z += v.z;
        }

        center.x /= vertices.Size();
        center.y /= vertices.Size();
        center.z /= vertices.Size();

        for (auto& v : vertices) {
            v.x = center.x + (v.x - center.x) * scale;
            v.y = center.y + (v.y - center.y) * scale;
            v.z = center.z + (v.z - center.z) * scale;
        }
    }

    Mesh& operator+=(const Mesh& src_mesh) {
        try {
            const int offset = static_cast<int>(vertices.Size());

            vertices.Reserve(vertices.Size() + src_mesh.vertices.Size());
            faces.Reserve(faces.Size() + src_mesh.faces.Size());

            for (const auto& v : src_mesh.vertices) {
                vertices.Add(v);
            }

            for (const auto& f : src_mesh.faces) {
                faces.Add(
                    Face(f.f1 + offset, f.f2 + offset, f.f3 + offset)
                );
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in operator+=: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in operator+="
                      << std::endl;
            throw;
        }
        return *this;
    }

    Mesh operator+(const Mesh& src_mesh) const {
        try {
            Mesh result = *this;
            result += src_mesh;
            return result;
        } catch (const std::exception& e) {
            std::cerr << "Exception in operator+: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in operator+"
                      << std::endl;
            throw;
        }
    }

    VertexContainer& getVertecies() {
        return vertices;
    }

    FaceContainer& getFaces() {
        return faces;
    }
};