#include "includes/imgui.h"
#include "appstate.h"
#include "game_engine.h"
#include "main_menu.h"

#include <GLFW/glfw3.h>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

static char inputBuffer[256] = "";

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;

bool no_name = false;
bool name_exists = false;
bool open_new_project = false;

using namespace std;

void CreateProject()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::Begin("Create Project", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Project Name:");
    ImGui::InputText("##InputName", inputBuffer, IM_ARRAYSIZE(inputBuffer));

    if (no_name) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Project name cannot be empty!");
    }

    if (name_exists) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Project name already exists!");
    }

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Back", ImVec2(200, 70)))
    {
        no_name = false;
        name_exists = false;
        currentAppState = AppState::MainMenu;
    }

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button("Create", ImVec2(200, 70)))
    {
        if (inputBuffer[0] == '\0') {
            no_name = true;
        }
        else {
            no_name = false;
            string projectName = string(inputBuffer) + ".txt";
            string projectPath = "projects/" + projectName;
            if(filesystem::exists(projectPath)) {
                name_exists = true;
            }
            else {
                name_exists = false;
                ofstream projectFile(projectPath);
                if (projectFile.is_open()) {
                    projectFile << projectName << endl;
                    projectFile.close();
                }
                glfwSetWindowShouldClose(main_window, true);
                open_new_project = true;
            }

        }
    }
    ImGui::End();
}

