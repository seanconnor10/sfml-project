#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class Camera {
public:
    sf::Vector3f position;
    sf::Vector2f rotation;
    float fov = 1.f;
};

#endif
