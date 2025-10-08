#pragma once

#include <glad.h>

static float cubeVertices[] = {
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
struct Cube {
    unsigned int VAO, VBO;
    float posx, posy, posz;
    float scalex, scaley, scalez;
    Cube() = default;
    Cube(float posx, float posy, float posz, float scalex, float scaley, float scalez) : posx(posx), posy(posy), posz(posz), scalex(scalex), scaley(scaley), scalez(scalez) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }
};