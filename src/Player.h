#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include "Camera.h"

class Player {

public:
    void placeCamera(Camera &camera);

    sf::Vector3f position;
    sf::Vector2f rotation;

    void step(float deltaTime);

private:
    void controls(float deltaTime);
};

#endif //PLAYER_H