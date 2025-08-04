#include<iostream>
#include <chrono>
#include <filesystem>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <sstream>
#include "includes/camera.h"
#include "includes/model.h"
#include "includes/imgui.h"
#include "includes/imgui_impl_glfw.h"
#include "includes/imgui_impl_opengl3.h"


using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//GUI

int StartNewProject()
{

    // glfw: initialize and configure
    // ------------------------------
    //IMGUI_CHECKVERSION(); already did
    //ImGui::CreateContext(); already did
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    //glfwInit(); already did
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //ImGui_ImplGlfw_InitForOpenGL(window,true); already did
    //ImGui_ImplOpenGL3_Init("#version 460"); already did

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // build and compile shaders
    // -------------------------
    Shader shader("src/shaders/vertexshad.vs", "src/shaders/fragmentshad.fs");
    Shader screenshad("src/shaders/screenvertexshad.vs", "src/shaders/screenfragmentshad.fs");
    Shader border("src/shaders/vertexshad.vs", "src/shaders/border.fs");
    Shader boxShader("src/shaders/boxvertexshader.vs", "src/shaders/boxfragmentshader.fs");
    Shader Modelshader("src/shaders/modelshader.vs", "src/shaders/modelshader.fs");
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
    vector<glm::vec3> windows;
    windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
    //unsigned int fbo;
    //glGenFramebuffers(1, &fbo);
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //unsigned int textureColorbuffer;
    //glGenTextures(1, &textureColorbuffer);
    //glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //skybox
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    cout << filesystem::current_path().string() << endl;
    unsigned char* data = stbi_load("src/resources/textures/marble.jpg", &width, &height, &nrComponents, 0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    unsigned int textureID2;
    glGenTextures(1, &textureID2);
    glBindTexture(GL_TEXTURE_2D, textureID2);
    cout << filesystem::current_path().string() << endl;
    data = stbi_load("src/resources/textures/metal.png", &width, &height, &nrComponents, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    unsigned int textureID3;
    glGenTextures(1, &textureID3);
    glBindTexture(GL_TEXTURE_2D, textureID3);
    data = stbi_load("src/resources/textures/pink_window.png", &width, &height, &nrComponents, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    vector<std::string> faces
    {
        "src/resources/textures/skybox/right.jpg",
            "src/resources/textures/skybox/left.jpg",
            "src/resources/textures/skybox/top.jpg",
            "src/resources/textures/skybox/bottom.jpg",
            "src/resources/textures/skybox/front.jpg",
            "src/resources/textures/skybox/back.jpg"
    };
    unsigned int boxtex;
    glGenTextures(1, &boxtex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, boxtex);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height,
            &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap failed to load at path: " << faces[i]
                << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
        GL_CLAMP_TO_EDGE);
    //textura ce biti fucked ako se ne flipuje

    stbi_set_flip_vertically_on_load(true);
    Model backpack("src/resources/objects/backpack/backpack.obj");
    stbi_set_flip_vertically_on_load(false);
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB,
        GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        texColorBuffer, 0);
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shader.use();
    shader.setInt("texture1", 0);
    boxShader.setInt("skybox", 0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();

        glStencilMask(0xFF);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        glStencilMask(0x00);
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID2);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        stbi_set_flip_vertically_on_load(true);
        Modelshader.use();
        Modelshader.setMat4("projection", projection);
        Modelshader.setMat4("view", view);
        Modelshader.setVec3("cameraPos", camera.Position);
        glBindTexture(GL_TEXTURE_CUBE_MAP, boxtex);
        glm::mat4 model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        Modelshader.setMat4("model", model);
        backpack.Draw(Modelshader);
        stbi_set_flip_vertically_on_load(false);
        shader.use();
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glStencilMask(0x00);
        shader.use();
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, textureID3);
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(camera.Position - windows[i]);
            sorted[distance] = windows[i];
        }
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it !=
            sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDepthFunc(GL_ALWAYS);
        border.use();
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        border.setMat4("view", view);
        border.setMat4("projection", projection);
        float scale = 1.1f;
        // cubes
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, textureID);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        border.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        border.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
        glStencilMask(0xFF);
        glDepthFunc(GL_LEQUAL);
         boxShader.use();
         view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
         projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
         boxShader.setMat4("view", view);
         boxShader.setMat4("projection", projection);
         glBindVertexArray(skyboxVAO);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_CUBE_MAP, boxtex);
         glDrawArrays(GL_TRIANGLES, 0, 36);
         glDepthFunc(GL_LESS);

        glDisable(GL_DEPTH_TEST);
        
        //screenshad.use();
        //glBindVertexArray(quadVAO);
        //glBindTexture(GL_TEXTURE_2D, texColorBuffer);	// use the color attachment texture as the texture of the quad plane
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}