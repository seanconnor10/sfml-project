#include <cmath>
#include <iostream>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Camera.h"
#include "Player.h"
#include "util.h"
#include "draw_helpers.h"

#define TEX_FILTER GL_LINEAR
#define PRINT_FPS TRUE
#define CULL_FACE TRUE

void draw3D(const Camera &camera, sf::Shader *shader, sf::Texture *texture, sf::Texture *texture2, int numWalls, std::vector<Plane> planes);

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "My window");

    std::cout << "OpenGLVersion: " << window.getSettings().majorVersion << "." << window.getSettings().minorVersion << std::endl;

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
    if (!testTexture.loadFromFile("../res/img/hex_steel.png")) {
        std::cout << "Failed to load texture";
        return EXIT_FAILURE;
    }
    sf::Texture testTexture2;
    if (!testTexture2.loadFromFile("../res/img/pixelwood.png")) {
        std::cout << "Failed to load texture";
        return EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST);
    if (CULL_FACE) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    sf::Shader::bind(&basicShader);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glDisableClientState(GL_COLOR_ARRAY);

    sf::Texture::bind(&testTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TEX_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TEX_FILTER);

    sf::Texture::bind(&testTexture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TEX_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TEX_FILTER);

    Player player = {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}};
    Camera camera = {player.position, player.rotation, 1.f};

    sf::Clock clock;
    bool running = true;
    float global_anim_factor = 0.f;
    float global_time_elapsed = 0.f;

     //diSector Map load
    /*std::vector<TwoPoints> pairs = makeWallSet("../res/cavern.txt");
    std::cout << "LOADED MAP WITH " << pairs.size() << " WALLS" << std::endl;
    const int NUM_WALLS = pairs.size() + 1;
    std::vector<Plane> planes;
    for (int i=0; i < NUM_WALLS - 1; i++) {
        auto [x1, z1, x2, z2] = pairs.at(i);
        planes.push_back(makePlane(x1, z1, x2, z2, &testTexture, 0, 30, 1, 1));
    }*/

    //TrenchBroom Map Load
    //PlaneArray planeArray = loadMap("../res/test.map", &testTexture);
    //std::cout << "LOADED MAP WITH " << planeArray.size << " WALLS" << std::endl;
    std::vector<Plane> planes = loadMap("../res/test.map", &testTexture);
    std::cout << "LOADED MAP WITH " << planes.size() << " WALLS" << std::endl;

    while (running)
    {
        float deltaTime = clock.restart().asSeconds();
        if (PRINT_FPS) {
            std::cout  << 1.f / deltaTime << std::endl;
        }

        global_time_elapsed += deltaTime;
        global_anim_factor = abs(std::cos(global_time_elapsed));

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

        basicShader.setUniform("lightFactor", global_anim_factor);
        basicShader.setUniform("lightPosition", sf::Glsl::Vec3(player.position));
        basicShader.setUniform("cameraPosition", sf::Glsl::Vec3(camera.position));

        //window.setActive(true);
        draw3D(camera, &basicShader, &testTexture, &testTexture2, planes.size(), planes);
        //window.setActive(false);

        window.display();
    }
}

void draw3D(const Camera &camera, sf::Shader *shader, sf::Texture *texture, sf::Texture *texture2, int numWalls, std::vector<Plane> planes) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat ratio = 800.f / 640.f;
    glFrustum(-ratio*camera.fov, ratio*camera.fov, -camera.fov, camera.fov, 1.f, 5000.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(camera.rotation.z, 0.f, 0.f, 1.f);
    glRotatef(camera.rotation.y, 1.f, 0.f, 0.f);
    glRotatef(camera.rotation.x, 0.f, 1.f, 0.f);
    glTranslatef(-camera.position.x, -camera.position.y, -camera.position.z);


    sf::Texture *previousTex = nullptr;

    //Plane2
    for (int i=0; i<numWalls; i++) {
        auto v = planes[i].vertices;
        /*if (previousTex != planes[i].tex && planes[i].tex != nullptr) {
            sf::Texture::bind(planes[i].tex); //Replace this with something faster
            previousTex = planes[i].tex;
        }*/
        const bool isFloor = (v[3] == 0 && v[4] == 1 && v[5] == 0); //Is Floor is normal points up
        sf::Texture *tex = isFloor ? texture2 : texture;
        if (previousTex != tex) {
            sf::Texture::bind(tex); //Replace this with something faster
            previousTex = tex;
        }
        glVertexPointer(3, GL_FLOAT, 8*sizeof(GLfloat), v);
        glNormalPointer(GL_FLOAT, 8*sizeof(GLfloat), v + 3);
        glTexCoordPointer(2, GL_FLOAT, 8*sizeof(GLfloat), v + 6);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}