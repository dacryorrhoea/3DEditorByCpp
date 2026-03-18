#pragma once

#include <SDL2/SDL.h>

struct Point2D {
    int x, y;
    float inv_z;
};

struct Polygon {
    Point2D p1;
    Point2D p2;
    Point2D p3;
    Uint32 color;
};