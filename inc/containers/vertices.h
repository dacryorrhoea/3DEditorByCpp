#pragma once

#include "sup_class/my_container.h"
#include "geometry/vertex.h"

class VertexContainer : public MyContainer<Vertex> {
public:
    using MyContainer<Vertex>::MyContainer;

    void AddVertex(const Vertex& vertex) {
        Add(vertex);
    }

    void ClearVertices() {
        Clear();
    }
};