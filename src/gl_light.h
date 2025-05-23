#pragma once

#include <SFML/Graphics/Glsl.hpp>

typedef struct {
    sf::Glsl::Vec3 position;
    sf::Glsl::Vec3 color;
    GLfloat falloffFactor;
} gl_Point_Light;

typedef struct {
    sf::Glsl::Vec3 position;
    sf::Glsl::Vec3 color;
    GLfloat falloffFactor;
} gl_Spot_Light;
