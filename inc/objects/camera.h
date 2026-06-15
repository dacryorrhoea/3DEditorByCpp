#pragma once

#include <algorithm>
#include <iostream>
#include <exception>
#include "containers/polygons.h"
#include "suph/all_objects.h"
#include "geometry/mesh.h"

class Camera : public Object {
private:
    bool active_state;
    int W;
    int H;
    float focal;
    PolygonContainer polygons;

    struct AxisData {
        Vertex dir;
        Vertex side1;
        Vertex side2;
        Uint32 color;
    };

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
        try {
            object_name = "Camera";
        } catch (const std::exception& e) {
            std::cerr << "Exception in Camera constructor: "
                      << e.what()
                      << std::endl;
            throw;
        } catch (...) {
            std::cerr << "Unknown exception in Camera constructor"
                      << std::endl;
            throw;
        }
    }

    bool getCamActiveState() const noexcept {
        return active_state;
    }

    void setCamActiveState(bool active) noexcept {
        active_state = active;
    }

    const PolygonContainer& getPolygons() const noexcept {
        return polygons;
    }

    int getWidth() const noexcept { return W; }
    int getHeight() const noexcept { return H; }

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