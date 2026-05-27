#pragma once

#include "scene/object.h"
#include "geometry/vertex.h"

struct LightSource : public Object {
private:
    bool active_state;

public:
    LightSource(int w, int h, bool active = false)
        : Object({0.0f, 0.0f, 0.0f}, false)
        , active_state(active)
    {
        forward = Vertex(-1.0f, -1.0f, -1.0f);
        right   = Vertex(1.0f, 0.0f, 0.0f);
        up      = Vertex(0.0f, 0.0f, 1.0f);
    }

    bool getLSActiveState() {
        return active_state;
    }

    void setLSActiveState(bool active) {
        active_state = active;
    }

    std::string getName() const override { return "LightSource"; }
};