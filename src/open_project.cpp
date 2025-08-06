#include "includes/imgui.h"
#include "includes/imgui_impl_glfw.h"
#include "includes/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <windows.h>

#include "game_engine.h"
#include "create_project.h"
#include "appstate.h"

int screenWidth;
int screenHeight;
int windowWidth;
int windowHeight;

AppState currentAppState = AppState::MainMenu;

void ShowStartWindow() {
    ImVec2 size = ImGui::GetWindowSize();
    ImGui::SetNextWindowPos(ImVec2(windowWidth/2-size.x/4, windowHeight/2-size.y/4), ImGuiCond_Always);
    ImGui::Begin("InvisibleWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Game Engine version 0.1.0");
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Start New Project", ImVec2(200, 40))) {
        currentAppState = AppState::CreateProject;
    }

    ImGui::Spacing();

    if (ImGui::Button("Load Project", ImVec2(200, 40))) {
        std::cout << "Load Project Clicked" << std::endl;
    }

    ImGui::Spacing();

    if (ImGui::Button("Settings", ImVec2(200, 40))) {
        std::cout << "Settings Clicked" << std::endl;
    }

    ImGui::End();
}

int main(int, char**)
{
    // Setup GLFW
    if (!glfwInit())
        return 1;

    // Setup OpenGL version (3.0+)
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    screenWidth = videoMode->width;
    screenHeight = videoMode->height;
    // Create window with graphics context
    windowWidth = screenWidth / 2;
    windowHeight = screenHeight / 2;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "engine", NULL, NULL);
    if (window == NULL)
        return 1;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    glfwSetWindowPos(window, posX, posY);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Our Start Window UI
        if(currentAppState == AppState::MainMenu) {
            ShowStartWindow();
        } else if (currentAppState == AppState::CreateProject) {
            CreateProject();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
