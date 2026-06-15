#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <exception>

#include "geometry/mesh.h"

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
    {
        try {
            //
        } catch (const std::exception& e) {
            std::cerr << "Exception in Object constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Object constructor"
                      << std::endl;
            throw;
        }
    }

    bool isChanged() const noexcept {
        return changed;
    }

    bool isRenderable() const noexcept {
        return renderable;
    }

    bool isEditeble() const noexcept {
        return editeble;
    }

    bool isSaveble() const noexcept {
        return saveble;
    }

    void moveForwardBackward(float dist) noexcept {
        position += forward * dist;
        changed = true;
    }

    void moveRightLeft(float dist) noexcept {
        position += right * dist;
        changed = true;
    }

    void moveUpDown(float dist) noexcept {
        position += up * dist;
        changed = true;
    }

    void rotateYaw(float angle);
    void rotatePitch(float angle);

    std::string getName() {
        try {
            return object_name;
        } catch (const std::exception& e) {
            std::cerr << "Exception in Object::getName: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Object::getName"
                      << std::endl;
            throw;
        }
    }

    virtual ~Object() = default;
};