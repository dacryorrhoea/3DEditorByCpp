#pragma once

#include <algorithm>
#include "containers/polygons.h"
#include "objects/all_objects.h"
#include "scene/object.h"
#include "geometry/mesh.h"

struct Camera : public Object {
private:
    bool active_state;
    int W;
    int H;
    float focal;
    PolygonContainer polygons;

public:
    Camera(int w, int h, bool active = false)
        : Object({0.5f, 0.5f, 80.0f}, false, false)
        , active_state(active)
        , W(w)
        , H(h)
        , focal(
            (w * 0.5f)
            / std::tan(
                (80.0f * 3.14159265358979323846f / 180.0f) * 0.5f
            )
        )
    {
        object_name = "Camera";
    }

    bool getCamActiveState() {
        return active_state;
    }

    void setCamActiveState(bool active) {
        active_state = active;
    }

    const PolygonContainer& getPolygons() const {
        return polygons;
    }

    std::vector<int> selectVertex(int x, int y, Object* obj);

    void getVerticesCamSpace(
        const VertexContainer& vertW,
        VertexContainer& vertCamSpace
    );

    void toProjectingScene(
        const std::vector<Object*>& objects,
        const LightSource& light_src
    );
};