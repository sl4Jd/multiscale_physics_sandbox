#pragma once

#include <glad.h>

unsigned int VAO_plane, VBO_plane;
unsigned int VAO_cube, VBO_cube;
unsigned int VAO_skybox, VBO_skybox;
unsigned int VAO_transparent, VBO_transparent;


float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
// front face
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
// left face
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
// right face
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
// bottom face
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
// top face
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
-0.5f, 0.5f, 0.5f, 0.0f, 0.0f // bottom-left
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float transparentVertices[] = {
        // positions         // texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f
    };
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -0.3f,  1.0f,  0.0f, 1.0f,
        -0.3f, 0.7f,  0.0f, 0.0f,
         0.3f, 0.7f,  1.0f, 0.0f,

        -0.3f,  1.0f,  0.0f, 1.0f,
         0.3f, 0.7f,  1.0f, 0.0f,
         0.3f,  1.0f,  1.0f, 1.0f
    };
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
struct Cube {
    float posx, posy, posz;
    float scalex, scaley, scalez;
};
struct Plane {
    float posx, posy, posz;
    float scalex, scaley, scalez;
};

void init_primitives_VAO_VBO() {
    // cube VAO/VBO
    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO_cube);
    glBindVertexArray(VAO_cube);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO/VBO
    glGenVertexArrays(1, &VAO_plane);
    glGenBuffers(1, &VBO_plane);
    glBindVertexArray(VAO_plane);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // transparent VAO/VBO
    glGenVertexArrays(1, &VAO_transparent);
    glGenBuffers(1, &VBO_transparent);
    glBindVertexArray(VAO_transparent);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_transparent);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //skybox VAO/VBO
    glGenVertexArrays(1, &VAO_skybox);
    glGenBuffers(1, &VBO_skybox);
    glBindVertexArray(VAO_skybox);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_skybox);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}