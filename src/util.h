#pragma once

#include <SFML/graphics.hpp>
#include <fstream>
#include <vector>
#include <string>
#include "draw_helpers.h"

typedef struct {
    float x1;
    float z1;
    float x2;
    float z2;
} TwoPoints;

typedef struct {
    int size;
    Plane planes[];
} PlaneArray;

inline std::string::size_type findAny(std::string& s, std::vector<std::string> tokens) {
    //Should only be single char strings for now...?
    //Finds earliest instance in string s of any string in tokens
    std::string::size_type earliest_pos = std::string::npos;
    for (std::string t : tokens) {
        std::string::size_type pos = s.find(t);
        if (pos != std::string::npos) {
            if (earliest_pos == std::string::npos || earliest_pos > pos) {
                earliest_pos = pos;
            }
        }
    }
    return earliest_pos;
}

inline std::vector<std::string> split(const std::string &s) {
    std::vector<std::string> delimiters;
    delimiters.push_back(" ");
    delimiters.push_back("\n");
    delimiters.push_back("\t");
    delimiters.push_back("\b");
    std::string line = s + " "; //Copy argument
    std::vector<std::string> tokens;
    size_t pos = 0;
    while ((pos = findAny(line, delimiters)) != std::string::npos) {
        std::string token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos+1);
    }
    //tokens.push_back(s);

    return tokens;
}

inline std::vector<Plane> loadMap(const std::string &filepath, sf::Texture *texture) {
    std::string current_line;
    std::ifstream file(filepath);

    std::vector<Plane> planes;

    if (file.is_open())
    {
        constexpr float SCALE = 0.5f;

        while(getline(file, current_line)) {
            //Shitty easy way of finding data...
            if (current_line.find("// brush") != std::string::npos) {
                //Skip "{" line
                getline(file, current_line);

                //Skip a face
                getline(file, current_line);

                //Front Face Line
                getline(file, current_line);
                std::vector<std::string> tokens = split(current_line);
                float x1 = std::stof(tokens[1])  * -SCALE;
                float z1 = std::stof(tokens[2])  * SCALE;
                float x2 = std::stof(tokens[6])  * -SCALE;
                float z2 = std::stof(tokens[7])  * SCALE;
                float y =  std::stof(tokens[13]) * SCALE;
                float height = std::stof(tokens[3]) - y;

                int h_repeat = std::stof(tokens[19]);
                int v_repeat = std::stof(tokens[20]);

                planes.push_back(makePlane(x1, z1, x2, z2, texture, y, height, h_repeat, v_repeat));

                //skip fce
                getline(file, current_line);

                //Top Face
                getline(file, current_line);
                tokens.clear();
                tokens = split(current_line);
                x1 = std::stof(tokens[1])  * -SCALE;
                z1 = std::stof(tokens[2])  * SCALE;
                x2 = std::stof(tokens[6])  * -SCALE;
                z2 = std::stof(tokens[12])  * SCALE;
                y =  std::stof(tokens[3]) * SCALE;

                h_repeat = std::stof(tokens[19]);
                v_repeat = std::stof(tokens[20]);

                planes.push_back(makeFloorPlane(x1,z1, x2,z2, texture, y, h_repeat, v_repeat));

            }
        }

    }

    return planes;

}

inline std::vector<TwoPoints> makeWallSet(const std::string& filepath) {
    std::string line;

    std::ifstream file(filepath);

    std::vector<TwoPoints> walls;

    if (file.is_open())
    {
        int wall_count = 0;
        std::vector<std::string> wall_lines = {};

        while(getline(file, line)) {
            std::cout << line << std::endl;
            std::string line_start = line.substr(0, 5);
            if (line_start == "WALL ") {
                wall_count++;
                wall_lines.push_back(line);
            }
        }
        std::cout << "Wall Count: " << wall_count << std::endl;

        int i=0;
        for (std::string s : wall_lines) {
            size_t position_data_start = s.find("Pos ");

            if (position_data_start == std::string::npos) {
                continue;
            }

            s = s.erase(0, position_data_start);

            std::vector<std::string> parts = split(s);

            if (parts.size() < 5) {
                continue;
            }

            constexpr float SCALE = 0.2f;

            float x1 = std::stof(parts[1]) * SCALE;
            float y1 = std::stof(parts[2]) * SCALE;
            float x2 = std::stof(parts[3]) * SCALE;
            float y2 = std::stof(parts[4]) * SCALE;

            //Were swapping the two points for correct normal direction
            walls.push_back({x2,y2, x1, y1});
        }

        file.close();

    }

    return walls;
}