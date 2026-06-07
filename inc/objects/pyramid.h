#pragma once

#include "object.h"
#include "geometry/mesh.h"

class Pyramid : public Object, public Mesh {
public:
    Pyramid()
        : Mesh(
            {
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,
                 0.0f,  0.5f,  0.0f
            },
            {
                0, 1, 2,
                0, 2, 3,
                0, 4, 1,
                1, 4, 2,
                2, 4, 3,
                3, 4, 0
            }
        )
    {
        object_name = "Pyramid";
        color = 0xFFFF22FF;
    }
};