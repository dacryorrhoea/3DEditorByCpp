#pragma once

#include <iostream>
#include <exception>
#include "object.h"
#include "geometry/mesh.h"

class Cube : public Object, public Mesh {
public:
    Cube()
    {
        try {
            const float h = 0.5f;

            vertices.Reserve(8);

            for (int z = -1; z <= 1; z += 2)
                for (int y = -1; y <= 1; y += 2)
                    for (int x = -1; x <= 1; x += 2)
                        vertices.Add(Vertex(x * h, y * h, z * h));

            faces.Reserve(12);

            const int quads[6][4] = {
                {0, 1, 3, 2},
                {4, 6, 7, 5},
                {0, 4, 5, 1},
                {2, 3, 7, 6},
                {1, 5, 7, 3},
                {0, 2, 6, 4}
            };

            for (const auto& q : quads) {
                faces.Add(Face(q[0], q[1], q[2]));
                faces.Add(Face(q[0], q[2], q[3]));
            }

            object_name = "Cube";
            color = 0xFFFF22FF;
        } catch (const std::exception& e) {
            std::cerr << "Exception in Cube constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Cube constructor"
                      << std::endl;
            throw;
        }
    }
};