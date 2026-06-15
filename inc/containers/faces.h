#pragma once

#include <iostream>
#include <exception>
#include "my_container.h"
#include "geometry/face.h"

class FaceContainer : public MyContainer<Face> {
public:
    using MyContainer<Face>::MyContainer;

    void AddFace(const Face& face) {
        try {
            Add(face);
        } catch (const std::exception& e) {
            std::cerr << "Exception in FaceContainer::AddFace: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in FaceContainer::AddFace"
                      << std::endl;
            throw;
        }
    }

    void ClearFaces() noexcept {
        try {
            Clear();
        } catch (const std::exception& e) {
            std::cerr << "Exception in FaceContainer::ClearFaces: "
                      << e.what()
                      << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception in FaceContainer::ClearFaces"
                      << std::endl;
        }
    }
};