//
// Created by Yam on 5/20/2025.
//

#include "Player.h"

#include <cmath>
#include <SFML/Window/Keyboard.hpp>

void Player::step(float deltaTime) {
    controls(deltaTime);
}

void Player::placeCamera(Camera &camera) {
    camera.position = {position.x, position.y, position.z};
    camera.rotation = {rotation.x, rotation.y};
}


void Player::controls(float deltaTime) {
    constexpr float DEG_TO_RAD = M_PI / 180;

    float cos = std::cos( DEG_TO_RAD* rotation.x);
    float sin = std::sin(DEG_TO_RAD*rotation.x);

    constexpr float SPEED = 20.f;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        position.x += cos * SPEED * deltaTime;
        position.z += sin * SPEED * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        position.x -= cos * SPEED * deltaTime;
        position.z -= sin * SPEED * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        position.x += sin * SPEED * deltaTime;
        position.z -= cos * SPEED * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        position.x -= sin * SPEED * deltaTime;
        position.z += cos * SPEED * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        position.y -= SPEED * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        position.y += SPEED * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        rotation.x -= 90.f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        rotation.x += 90.f * deltaTime;
    }
}