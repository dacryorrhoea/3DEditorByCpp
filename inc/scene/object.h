#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "geometry/mesh.h"
#include "geometry/vertex.h"


class Object {
protected:
    std::string object_name;

    bool changed;
    bool renderable;
    bool editeble;
    bool saveble;

public:
    Vertex position;
    Vertex forward;
    Vertex right;
    Vertex up;

    Uint32 color;

    Object(
        const Vertex& pos = {},
        bool rndrbl = true,
        bool editbl = true,
        bool savebl = false
    )
        : object_name("Just object")
        , changed(false)
        , renderable(rndrbl)
        , editeble(editbl)
        , saveble(savebl)
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

    bool isEditeble() {
        return editeble;
    }

    bool isSaveble() {
        return saveble;
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


    std::string getName() {
        return object_name;
    };

    virtual ~Object() = default;
};