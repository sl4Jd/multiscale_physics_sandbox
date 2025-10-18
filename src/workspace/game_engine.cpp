#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <sstream>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <json.hpp>

#include "camera.h"
#include "model.h"
#include "create_project.h"
#include "save_project.h"
#include "primitives.h"
#include "toolbar.h"

using json = nlohmann::json;
using namespace std;

struct Object {
    string name;
    string type;
    string shape;
    float posx, posy, posz;
    float scalex, scaley, scalez;
    string texture;
};
vector<Object> objects;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

json project_settings;

extern GLFWwindow* main_window;

extern unsigned int VAO_cube, VBO_cube;
extern unsigned int VAO_plane, VBO_plane;
extern unsigned int VAO_skybox, VBO_skybox;
extern unsigned int VAO_transparent, VBO_transparent;

void get_settings(){
    ifstream file("user_data/projects/working/scene.json");
    file >> project_settings;
    for (auto& obj : project_settings["objects"]) {
        objects.push_back({ obj["name"], 
                            obj["type"], 
                            obj["shape"], 
                            obj["posx"].get<float>(), 
                            obj["posy"].get<float>(), 
                            obj["posz"].get<float>(), 
                            obj["scalex"].get<float>(), 
                            obj["scaley"].get<float>(), 
                            obj["scalez"].get<float>(),
                            obj["texture"].get<string>()
        });
    }
}
void StartNewProject()
{

    // glfw: initialize and configure
    // ------------------------------
    //IMGUI_CHECKVERSION(); already did
    //ImGui::CreateContext(); already did
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImGui::StyleColorsDark();
    //glfwInit(); already did
    get_settings();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "project", NULL, main_window);
    if (window == NULL)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Failed to initialize GLAD" << endl;
        return;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);
    
    // build and compile shaders
    Shader shader("assets/shaders/vertexshad.vs", "assets/shaders/fragmentshad.fs");
    Shader screenshad("assets/shaders/screenvertexshad.vs", "assets/shaders/screenfragmentshad.fs");
    Shader border("assets/shaders/vertexshad.vs", "assets/shaders/border.fs");
    Shader boxShader("assets/shaders/boxvertexshader.vs", "assets/shaders/boxfragmentshader.fs");
    Shader Modelshader("assets/shaders/modelshader.vs", "assets/shaders/modelshader.fs");

    // windows locaions
    vector<glm::vec3> windows;
    windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    // configure cube VAO/VBO, plane VAO/VBO, transparent VAO/VBO, skybox VAO/VBO
    init_primitives_VAO_VBO();

    unordered_map<string, unsigned int*> texs;

    // load marble texture
    unsigned int marble_tex;
    glGenTextures(1, &marble_tex);
    int width, height, nrComponents;
    unsigned char* data = stbi_load("assets/resources/textures/marble.jpg", &width, &height, &nrComponents, 0);
    glBindTexture(GL_TEXTURE_2D, marble_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    texs["marble_tex"] = &marble_tex;

    // load metal texture
    unsigned int metal_tex;
    glGenTextures(1, &metal_tex);
    glBindTexture(GL_TEXTURE_2D, metal_tex);
    data = stbi_load("assets/resources/textures/metal.png", &width, &height, &nrComponents, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cerr << "Failed to load texture" << endl;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    texs["metal_tex"] = &metal_tex;

    // load window texture
    unsigned int pink_window_tex;
    glGenTextures(1, &pink_window_tex);
    glBindTexture(GL_TEXTURE_2D, pink_window_tex);
    data = stbi_load("assets/resources/textures/pink_window.png", &width, &height, &nrComponents, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cerr << "Failed to load texture" << endl;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    texs["pink_window_tex"] = &pink_window_tex;

    // load skybox textures
    string path = "assets/resources/textures/"+ project_settings["skybox"].get<string>() + "/";
    vector<string> faces
    {
        path+"right.jpg",
        path+"left.jpg",
        path+"top.jpg",
        path+"bottom.jpg",
        path+"front.jpg",
        path+"back.jpg"
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
            cerr << "Cubemap failed to load at path: " << faces[i]
                << endl;
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
    //textura ce biti fucked ako se ne flipuje za modele

    map<string, Model*> sceneModels;
    unordered_map<string, Cube> cubes;
    unordered_map<string, Plane> planes;
    for (const auto& obj : objects) {
        if (obj.type == "model") {
            stbi_set_flip_vertically_on_load(true);
            sceneModels[obj.name] = new Model("assets/resources/objects/" + obj.shape + "/"+ obj.shape + ".obj");
            stbi_set_flip_vertically_on_load(false);
        }
        else if (obj.type == "primitive") {
            if (obj.shape == "cube") {
                cubes[obj.name] = Cube{obj.posx, obj.posy, obj.posz, obj.scalex, obj.scaley, obj.scalez, obj.texture};
            }
            else if (obj.shape == "plane") {
                planes[obj.name] = Plane{obj.posx, obj.posy, obj.posz, obj.scalex, obj.scaley, obj.scalez, obj.texture};
            }
        }
    }
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1400, 900, 0, GL_RGB,
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
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1400, 900);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glStencilMask(0xFF);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 


        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        glStencilMask(0x00);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO_plane);
        for (const auto& plane : planes) {
            glBindTexture(GL_TEXTURE_2D, *texs[plane.second.texture]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(plane.second.posx, plane.second.posy, plane.second.posz));
            model = glm::scale(model, glm::vec3(plane.second.scalex, plane.second.scaley, plane.second.scalez));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
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
        for (const auto& obj : sceneModels) {
            obj.second->Draw(Modelshader);
        }
        stbi_set_flip_vertically_on_load(false);
        shader.use();
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        // cubes
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO_cube);
        for (auto& cube : cubes) {
            glBindTexture(GL_TEXTURE_2D, *texs[cube.second.texture]);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(cube.second.posx, cube.second.posy, cube.second.posz));
            model = glm::scale(model, glm::vec3(cube.second.scalex, cube.second.scaley, cube.second.scalez));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glStencilMask(0x00);
        shader.use();
        glBindVertexArray(VAO_transparent);
        glBindTexture(GL_TEXTURE_2D, *texs["pink_window_tex"]);
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
        // cubes border
        float scale = 1.1f;
        for (auto& cube : cubes) {
            glBindVertexArray(VAO_cube);
        }
        glBindTexture(GL_TEXTURE_2D, marble_tex);
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
        glBindVertexArray(VAO_skybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, boxtex);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        Toolbar();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    glfwTerminate();
    return;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //save and quit
        SaveProject(project_settings["name"]);
        glfwSetWindowShouldClose(window, true);
    }

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
bool dragging = false;
float save_x,save_y;
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

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(!dragging){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            dragging = true;
            save_x = xpos;
            save_y = ypos;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else if(dragging){
        dragging = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPos(window, save_x, save_y);
    }


    lastX = xpos;
    lastY = ypos;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}