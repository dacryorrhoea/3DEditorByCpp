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

            // Основание: обход против часовой стрелки (если смотреть сверху)
            vertices.Add(Vertex(-h, -h, -h)); // 0
            vertices.Add(Vertex( h, -h, -h)); // 1
            vertices.Add(Vertex( h, -h,  h)); // 2
            vertices.Add(Vertex(-h, -h,  h)); // 3

            // Вершина пирамиды
            vertices.Add(Vertex(0.0f, h, 0.0f)); // 4

            faces.Reserve(6);

            // Нижнее основание (два треугольника, нормали вниз)
            faces.Add(Face(0, 2, 1)); // 0-2-1
            faces.Add(Face(0, 3, 2)); // 0-3-2

            // Боковые грани (нормали наружу)
            for (int i = 0; i < 4; ++i)
                faces.Add(Face(i, (i + 1) % 4, 4)); // i -> (i+1)%4 -> 4

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