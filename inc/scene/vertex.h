#pragma once

struct vertex {
    std::array<float,3> v;

    Vertex() : v{0.0f, 0.0f, 0.0f} {}
    Vertex(float x, float y, float z) : v{x, y, z} {} // member initializer list

    void setV(float x, float y, float z) {
        v[0]=x;
        v[1]=y;
        v[2]=z;
    }

    void transformV(float* matrix) {
        float x = v[0], y = v[1], z = v[2], w = 1.0f;

        float newX = matrix[0]*x + matrix[1]*y + matrix[2]*z + matrix[3]*w;
        float newY = matrix[4]*x + matrix[5]*y + matrix[6]*z + matrix[7]*w;
        float newZ = matrix[8]*x + matrix[9]*y + matrix[10]*z + matrix[11]*w;
        
        v[0] = newX;
        v[1] = newY;
        v[2] = newZ;
    };
};
