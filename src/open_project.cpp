#include "includes/imgui.h"
#include "includes/imgui_impl_glfw.h"
#include "includes/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <windows.h>

#include "game_engine.h"
#include "create_project.h"
#include "open_project.h"
#include "appstate.h"

int screenWidth;
int screenHeight;
int windowWidth;
int windowHeight;

extern bool open_project;

AppState currentAppState = AppState::MainMenu;

GLFWwindow* main_window;

void ShowStartWindow() {
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin("InvisibleWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SetCursorPosY(100);
    ImGui::SetCursorPosX(windowWidth/2 - 250);
    ImGui::Text("Game Engine version 0.1.1");
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::SetCursorPosX(windowWidth/2 - 250);
    if (ImGui::Button("Start New Project", ImVec2(500, 100))) {
        currentAppState = AppState::CreateProject;
    }

    ImGui::Spacing();

    ImGui::SetCursorPosX(windowWidth/2 - 250);
    if (ImGui::Button("Load Project", ImVec2(500, 100))) {
        std::cout << "Load Project Clicked" << std::endl;
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX(windowWidth/2 - 250);
    if (ImGui::Button("Settings", ImVec2(500, 100))) {
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
    main_window = glfwCreateWindow(windowWidth, windowHeight, "engine", NULL, NULL);
    if (main_window == NULL)
        return 1;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    glfwSetWindowPos(main_window, posX, posY);
    glfwMakeContextCurrent(main_window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImFont* lucida_big = io.Fonts->AddFontFromFileTTF("src\\fonts\\lucon.ttf", 32.0f);
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(main_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    while (!glfwWindowShouldClose(main_window))
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
        glfwGetFramebufferSize(main_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(main_window);
    }
    
    glfwDestroyWindow(main_window);
    if(open_project) {
        StartNewProject();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
