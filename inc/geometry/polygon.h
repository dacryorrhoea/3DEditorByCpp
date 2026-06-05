#pragma once

#include <SDL2/SDL.h>
// #include 

struct Polygon {
    struct Point {
        int x;
        int y;
        float inv_z;
    };

    int sv1;
    int sv2;
    int sv3;
    
    Point p1;
    Point p2;
    Point p3;

    Uint32 solid_color;
    Uint32 flat_shading;
};