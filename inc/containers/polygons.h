#pragma once

#include "sup_class/my_container.h"
#include "geometry/vertex.h"
#include "geometry/polygon.h"

class PolygonContainer : public MyContainer<Polygon> {
public:
    void Add(const Vertex& v1,
             const Vertex& v2,
             const Vertex& v3,
             const int sv1,
             const int sv2,
             const int sv3,
             const float focal,
             const int HW,
             const int HH,
             Uint32 solid_color,
             Uint32 flat_shading) 
    {
        // отсечение некоторого количества граней
        const float nearPlane = 0.1f;

        if (v1.z <= nearPlane || v2.z <= nearPlane || v3.z <= nearPlane)
            return;

        // Vertex n;
        // n = (v2 - v1).cross(v3 - v1);

        // if (std::abs(n.x) + std::abs(n.y) + std::abs(n.z) < 1e-6)
        //     return;

        // if (n.dot(v1) >= 0)
        //     return;

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
    }
};