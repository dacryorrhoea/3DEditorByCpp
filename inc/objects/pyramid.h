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

            vertices.Add(Vertex(-h, -h, -h));
            vertices.Add(Vertex( h, -h, -h));
            vertices.Add(Vertex( h, -h,  h));
            vertices.Add(Vertex(-h, -h,  h));

            vertices.Add(Vertex(0.0f, h, 0.0f));

            faces.Reserve(6);

            faces.Add(Face(0, 2, 1));
            faces.Add(Face(0, 3, 2));

            for (int i = 0; i < 4; ++i)
                faces.Add(Face(i, (i + 1) % 4, 4));

            object_name = "Pyramid";
            color = 0xFFFF22FF;
        } catch (const std::exception& e) {
            std::cerr << "Exception in Pyramid constructor: "
                      << e.what() << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Pyramid constructor"
                    << std::endl;
            throw;
        }
    }
};