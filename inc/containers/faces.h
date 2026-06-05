#pragma once

#include "my_container.h"
#include "geometry/face.h"

class FaceContainer : public MyContainer<Face> {
public:
    using MyContainer<Face>::MyContainer;

    void AddFace(const Face& face) {
        Add(face);
    }

    void ClearFaces() {
        Clear();
    }
};