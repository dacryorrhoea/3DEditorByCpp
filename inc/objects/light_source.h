#pragma once

#include <iostream>
#include <exception>
#include "object.h"

struct LightSource : public Object {
private:
    bool active_state;

public:
    LightSource(int w, int h, bool active = false)
        : Object({0.0f, 0.0f, 0.0f}, false, false, false)
        , active_state(active)
    {
        try {
            object_name = "LightSource";
            forward = Vertex(-1.0f, -1.0f, -1.0f);
            right   = Vertex(1.0f, 0.0f, 0.0f);
            up      = Vertex(0.0f, 0.0f, 1.0f);
        } catch (const std::exception& e) {
            std::cerr << "Exception in LightSource constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in LightSource constructor"
                      << std::endl;
            throw;
        }
    }

    bool getLSActiveState() const noexcept {
        return active_state;
    }

    void setLSActiveState(bool active) noexcept {
        active_state = active;
    }
};