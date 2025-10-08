#include "primitives.h"
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
static float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
static float transparentVertices[] = {
        // positions         // texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f
    };
static float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -0.3f,  1.0f,  0.0f, 1.0f,
        -0.3f, 0.7f,  0.0f, 0.0f,
         0.3f, 0.7f,  1.0f, 0.0f,

        -0.3f,  1.0f,  0.0f, 1.0f,
         0.3f, 0.7f,  1.0f, 0.0f,
         0.3f,  1.0f,  1.0f, 1.0f
    };
static float skyboxVertices[] = {
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

    Cube::Cube() {
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