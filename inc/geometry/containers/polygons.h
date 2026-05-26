#pragma once

#include "sup_class/my_container.h"
#include "polygon.h"

class PolygonContainer : public MyContainer<Polygon>
{
public:
    void Add(const Polygon& polygon) override
    {

        storage.push_back(polygon);
    }
};