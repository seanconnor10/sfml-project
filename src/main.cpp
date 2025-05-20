#include <iostream>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Camera.h"
#include "Player.h"

void draw3D(const Camera &camera, sf::Shader *shader, const sf::Texture *texture);

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "My window");

    if (!window.setActive(true)) {
        std::cout << "Failed to activate Window";
        return EXIT_FAILURE;
    }

    sf::Shader basicShader;
    if (!basicShader.loadFromFile("../res/shaders/basicVERT.txt", "../res/shaders/basicFRAG.txt")) {
        std::cout << "Failed to load shader";
        return EXIT_FAILURE;
    }

    sf::Texture testTexture;
    if (!testTexture.loadFromFile("../res/img/pixelwood.png")) {
        std::cout << "Failed to load texture";
        return EXIT_FAILURE;
    }

    sf::Texture::bind(&testTexture);
    sf::Shader::bind(&basicShader);
    basicShader.setUniform("lightFactor", 1.0f );

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glEnable(GL_DEPTH_TEST);

    Player player = {{0.f, 0.f, 0.f}, {0.f, 0.f}};
    Camera camera = {player.position, player.rotation, 1.f};

    sf::Clock clock;
    bool running = true;
    while (running)
    {
        sf::Time timeElapsed = clock.restart();
        float deltaTime = timeElapsed.asSeconds();
        std::cout  << 1.f / deltaTime << std::endl;
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()) {
                running = false;
            }  else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
        }

        player.step(deltaTime);
        player.placeCamera(camera);

        //window.setActive(true);
        draw3D(camera, &basicShader, &testTexture);
        //window.setActive(false);

        window.display();
    }
}

void draw3D(const Camera &camera, sf::Shader *shader, const sf::Texture *texture) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat ratio = 800.f / 640.f;
    glFrustum(-ratio*camera.fov, ratio*camera.fov, -camera.fov, camera.fov, 1.f, 5000.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(camera.rotation.y, 0.f, 0.f, 1.f);
    glRotatef(camera.rotation.x, 0.f, 1.f, 0.f);
    glTranslatef(-camera.position.x, -camera.position.y, -camera.position.z);

    const GLfloat basicPlaneVerts[] = {
        0, -10, -10, 0,0,
        0, -10,  10, 0,1,
        0,  10, -10, 1,0,

        0,  10,  10, 1,1,
        0, -10,  10, 0,1,
        0,  10, -10, 1,0,

        0, -10, -10, 0,0,
        0, -10,  10, 0,1,
        0,  10, -10, 1,0,

         0,  10,  10, 1,1,
         0, -10,  10, 0,1,
        0,  10, -10, 1,0,

        //Plane 2
        10, -10, -10, 0,0,
        10, -10,  10, 0,1,
        10,  10, -10, 1,0,

        10,  10,  10, 1,1,
        10, -10,  10, 0,1,
        10,  10, -10, 1,0,

        10, -10, -10, 0,0,
        10, -10,  10, 0,1,
        10,  10, -10, 1,0,

        10,  10,  10, 1,1,
        10, -10,  10, 0,1,
        10,  10, -10, 1,0
    };

    glVertexPointer(3, GL_FLOAT, 5*sizeof(GLfloat), basicPlaneVerts);
    glTexCoordPointer(2, GL_FLOAT, 5*sizeof(GLfloat), basicPlaneVerts + 3);
    glDrawArrays(GL_TRIANGLES, 0, 12);

}