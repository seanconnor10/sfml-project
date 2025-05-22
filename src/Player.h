#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include "Camera.h"

class Player {

public:
    sf::Vector3f position;
    sf::Vector3f rotation;
    sf::Vector3f velocity;

    void step(float deltaTime);
    void placeCamera(Camera &camera);

private:
    void controls(float deltaTime);
    static sf::Vector2f getInputVector();
};

#endif //PLAYER_H