#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include "my_container.h"
#include "geometry/vertex.h"
#include "geometry/polygon.h"

class PolygonContainer : public MyContainer<Polygon> {
public:
    void Add(const Vertex& v1,
             const Vertex& v2,
             const Vertex& v3,
             const float focal,
             const int HW,
             const int HH,
             Uint32 solid_color,
             Uint32 flat_shading) 
    {
        try {
            if (v1.z == 0.0f || v2.z == 0.0f || v3.z == 0.0f) {
                throw std::runtime_error(
                    "Division by zero in PolygonContainer::Add"
                );
            }

            Polygon pol;
            pol.p1.x = static_cast<int>((v1.x / v1.z) * focal + HW + 0.5f);
            pol.p1.y = static_cast<int>(-(v1.y / v1.z) * focal + HH + 0.5f);
            pol.p1.inv_z = 1 / v1.z;

            pol.p2.x = static_cast<int>((v2.x / v2.z) * focal + HW + 0.5f);
            pol.p2.y = static_cast<int>(-(v2.y / v2.z) * focal + HH + 0.5f);
            pol.p2.inv_z = 1 / v2.z;

            pol.p3.x = static_cast<int>((v3.x / v3.z) * focal + HW + 0.5f);
            pol.p3.y = static_cast<int>(-(v3.y / v3.z) * focal + HH + 0.5f);
            pol.p3.inv_z = 1 / v3.z;

            pol.solid_color = solid_color;
            pol.flat_shading = flat_shading;

            storage.push_back(pol);
        } catch (const std::exception& e) {
            std::cerr << "Exception in PolygonContainer::Add: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in PolygonContainer::Add"
                      << std::endl;
            throw;
        }
    }

    void ClearPolygons() noexcept {
        try {
            Clear();
        } catch (const std::exception& e) {
            std::cerr << "Exception in ClearPolygons: "
                      << e.what()
                      << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception in ClearPolygons"
                      << std::endl;
        }
    }
};