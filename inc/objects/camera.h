#pragma once

#include "sup_class/custom_types.h"
#include "scene/object.h"
#include "geometry/mesh.h"
#include "geometry/vertex.h"

struct Camera : public Object {
private:
    bool active_state;
    int W;
    int H;
    float focal;
    std::vector<Polygon> polygons;
public:
    Camera(int w, int h, bool active = false)
        : Object({0.5f, 0.5f, 80.0f}, false)
        , active_state(active)
        , W(w)
        , H(h)
        , focal( (w * 0.5f) / std::tan( (80.0f * 3.14159265358979323846f / 180.0f) * 0.5f ) )
    {}

    bool getCamActiveState() {
        return active_state;
    }

    void setCamActiveState(bool active) {
        active_state = active;
    }

    const std::vector<Polygon>& getPolygons() const {
        return polygons;
    }

    std::string getName() const override { return "Camera"; }

    void toProjectingScene(const std::vector<Object*>& objects);
};