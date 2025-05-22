#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

typedef struct {
    sf::Texture *tex;
    GLfloat vertices[48];
} Plane;

inline Plane makePlane(float x1, float z1, float x2, float z2, sf::Texture *tex, float y, float height, int h_repeat, int v_repeat) {
    const auto vr = static_cast<float>(v_repeat);
    const auto hr = static_cast<float>(h_repeat);

    const float nx = z1 - z2;
    const float nz = x2 - x1;
    constexpr float ny = 0;

    return {
        tex,
        {   //Pos                //Norm        //UV
            x1, y,        z1,    nx, ny, nz,   0,0,
            x2, y,        z2,    nx, ny, nz,   hr,0,
            x1, y+height, z1,    nx, ny, nz,   0,vr,

            x1, y+height, z1,    nx, ny, nz,   0,vr,
            x2, y,        z2,    nx, ny, nz,   hr,0,
            x2, y+height, z2,    nx, ny, nz,   hr,vr

        }
    };
}

inline Plane makeFloorPlaneCenter(float size, sf::Texture *tex, float repeat) {
    return {
        tex,
        {   //Pos               //Norm     //UV
            -size, 0, -size,    0, 1, 0,   -repeat,-repeat,
            -size, 0,  size,    0, 1, 0,   -repeat, repeat,
             size, 0, -size,    0, 1, 0,    repeat,-repeat,


            -size, 0,  size,    0, 1, 0,  -repeat, repeat,
            size, 0,  size,    0, 1, 0,   repeat, repeat,
             size, 0, -size,    0, 1, 0,   repeat,-repeat

        }
    };
}

inline Plane makeFloorPlane(float x1, float z1, float x2, float z2, sf::Texture *tex, float y, int h_repeat, int v_repeat) {
    const auto vr = static_cast<float>(v_repeat);
    const auto hr = static_cast<float>(h_repeat);
    return {
        tex,
        {
            //Pos         //Norm     //UV
            x1, y, z1,    0, 1, 0,   0,  0,
            x1, y, z2,    0, 1, 0,   0,  vr,
            x2, y, z1,    0, 1, 0,   hr, 0,

            x1, y, z2,    0, 1, 0,   0,  vr,
            x2, y, z2,    0, 1, 0,   hr, vr,
            x2, y, z1,    0, 1, 0,   hr, 0

        }
    };
}