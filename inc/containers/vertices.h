#pragma once

#include <iostream>
#include <exception>
#include "my_container.h"
#include "geometry/vertex.h"

class VertexContainer : public MyContainer<Vertex> {
public:
    using MyContainer<Vertex>::MyContainer;

    void AddVertex(const Vertex& vertex) {
        try {
            Add(vertex);
        } catch (const std::exception& e) {
            std::cerr << "Exception in VertexContainer::AddVertex: "
                      << e.what() << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in VertexContainer::AddVertex"
                      << std::endl;
            throw;
        }
    }

    void ClearVertices() noexcept {
        try {
            Clear();
        } catch (const std::exception& e) {
            std::cerr << "Exception in VertexContainer::ClearVertices: "
                      << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception in VertexContainer::ClearVertices"
                      << std::endl;
        }
    }
};