#pragma once

#include <vector>

class Cube {

private:

std::vector<float*> faces;
std::vector<int*> ibuffers;

public:

enum class Face {
    UP = 0, 
    DOWN,
    FRONT,
    BOTTOM,
    LEFT,
    RIGHT
};

Cube();
~Cube();
// essentially an abstraction for finding an index of a vector
void getFace(float * face, int * indices, Face f);

/*
float positions[] = {
        // 3D            Textures
        -25,-25, -25,    0.0f, 0.0f, // 0 Back
         25,-25, -25,    1.0f, 0.0f, // 1
         25, 25, -25,    1.0f, 1.0f, // 2
        -25, 25, -25,    0.0f, 1.0f, // 3
        -25,-25,  25,    0.0f, 0.0f, // 4 Front
         25,-25,  25,    1.0f, 0.0f, // 5
         25, 25,  25,    1.0f, 1.0f, // 6
        -25, 25,  25,    0.0f, 1.0f  // 7
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // back face
        4, 5, 6, 6, 7, 4, // front face
        0, 1, 5, 5, 4, 0, // bottom face
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1
    };
*/

};