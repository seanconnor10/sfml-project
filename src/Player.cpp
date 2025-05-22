//
// Created by Yam on 5/20/2025.
//

#include "Player.h"

#include <SFML/Window/Keyboard.hpp>

void Player::step(float deltaTime) {
    controls(deltaTime);

    position += velocity * deltaTime;
}

void Player::placeCamera(Camera &camera) {
    camera.position = position;
    camera.rotation = rotation;
}

void Player::controls(float deltaTime) {
    constexpr float SPEED = 80.f;
    constexpr float ACCEL = 30.f;
    const float speedTime = SPEED * deltaTime;
    const float accelTime = ACCEL * ACCEL * deltaTime;

    sf::Vector2f inputVector = getInputVector();
    if (inputVector != sf::Vector2f{0.f, 0.f}) {
        inputVector = inputVector.normalized().rotatedBy(sf::degrees(rotation.x));
    }

    velocity.x += inputVector.x * accelTime;
    velocity.z += inputVector.y * accelTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        position.y -= speedTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        position.y += speedTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        rotation.x -= 90.f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        rotation.x += 90.f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        rotation.y -= 90.f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        rotation.y += 90.f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        rotation.z -= 90.f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        rotation.z += 90.f * deltaTime;
    }

    velocity *= 1.f - 5.f*deltaTime;
    if (velocity.length() > SPEED) {
        velocity *= SPEED / velocity.length();
    }
}

sf::Vector2f Player::getInputVector() {
    sf::Vector2f vector = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        vector.x += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        vector.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        vector.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        vector.y += 1.f;
    }

    return vector;
}