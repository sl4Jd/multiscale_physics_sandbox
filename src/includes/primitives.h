#pragma once

#include <glad.h>
#include <string>

using namespace std;

struct Cube {
    float posx, posy, posz;
    float scalex, scaley, scalez;
    string texture;
};
struct Plane {
    float posx, posy, posz;
    float scalex, scaley, scalez;
    string texture;
};

void init_primitives_VAO_VBO();