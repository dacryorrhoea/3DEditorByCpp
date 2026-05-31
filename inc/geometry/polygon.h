#pragma once

#include <SDL2/SDL.h>

struct Polygon {
    struct Point {
        int x;
        int y;
        float inv_z;
    };
    
    Point p1;
    Point p2;
    Point p3;

    Uint32 solid_color;
    Uint32 flat_shading;
};