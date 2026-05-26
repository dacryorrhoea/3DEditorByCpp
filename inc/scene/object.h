#pragma once

#include <SDL2/SDL.h>

#include "geometry/mesh.h"
#include "geometry/vertex.h"


class Object {
protected:
    bool changed;
    bool renderable;

public:
    Vertex position;
    Vertex forward;
    Vertex right;
    Vertex up;

    Uint32 color;

    Object(const Vertex& pos = {}, bool rndrbl = true)
        : changed(false)
        , renderable(rndrbl)
        , position(pos)
        , forward(0.0f, 0.0f, -1.0f)
        , right(1.0f, 0.0f, 0.0f)
        , up(0.0f, 1.0f, 0.0f)
        , color(0x808080FF)
    {}

    bool isChanged() {
        return changed;
    }

    bool isRenderable() {
        return renderable;
    }

    void moveForwardBackward(float dist) {
        position += forward * dist;
        changed = true;
    }

    void moveRightLeft(float dist) {
        position += right * dist;
        changed = true;
    }

    void moveUpDown(float dist) {
        position += up * dist;
        changed = true;
    }

    void rotateYaw(float angle);
    void rotatePitch(float angle);

    virtual std::string getName() const = 0;

    virtual ~Object() = default;
};