#pragma once

#include <iostream>
#include <exception>
#include "object.h"
#include "geometry/mesh.h"

class Pyramid : public Object, public Mesh {
public:
    Pyramid()
    {
        try {
            const float h = 0.5f;

            vertices.Reserve(5);

            for (int z = -1; z <= 1; z += 2)
                for (int x = -1; x <= 1; x += 2)
                    vertices.Add(Vertex(x * h, -h, z * h));

            vertices.Add(Vertex(0.0f, h, 0.0f));

            faces.Reserve(6);

            faces.Add(Face(0, 1, 3));
            faces.Add(Face(0, 3, 2));

            for (int i = 0; i < 4; ++i)
                faces.Add(Face(i, 4, (i + 1) % 4));

            object_name = "Pyramid";
            color = 0xFFFF22FF;
        } catch (const std::exception& e) {
            std::cerr << "Exception in Pyramid constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Pyramid constructor"
                      << std::endl;
            throw;
        }
    }
};