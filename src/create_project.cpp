#include <imgui/imgui.h>
#include "appstate.h"
#include "game_engine.h"
#include "main_menu.h"

#include <GLFW/glfw3.h>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <miniaudio/miniaudio.h>

static char inputBuffer[32] = "";

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;
extern ma_engine engine;
extern const char* click_sound;
extern const char* hover_sound;

bool no_name = false;
bool name_exists = false;
bool open_new_project = false;

static bool some_hovered = false;
static bool some_was_hovered = false;

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

    some_hovered = false;
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Back", ImVec2(200, 70)))
    {
        ma_engine_play_sound(&engine, click_sound, NULL);
        no_name = false;
        name_exists = false;
        currentAppState = AppState::MainMenu;
    }
    if(ImGui::IsItemHovered())
    {
        some_hovered = true;
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button("Create", ImVec2(200, 70)))
    {
        ma_engine_play_sound(&engine, click_sound, NULL);
        if (inputBuffer[0] == '\0') {
            no_name = true;
        }
        else {
            no_name = false;
            string projectName = string(inputBuffer);
            string projectPath = "projects/" + projectName + ".txt";
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
    if(ImGui::IsItemHovered())
    {
        some_hovered = true;
    }
    if(some_hovered && !some_was_hovered) {
        ma_engine_play_sound(&engine, hover_sound, NULL);
    }
    some_was_hovered = some_hovered;
    ImGui::End();
}

