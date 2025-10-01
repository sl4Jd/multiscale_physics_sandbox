#include <imgui/imgui.h>
#include "appstate.h"
#include "game_engine.h"
#include "main.h"
#include "create_project.h"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <miniaudio/miniaudio.h>

using namespace std;

static char inputBuffer[32] = "";

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;

extern ma_sound clickSound;
extern ma_sound hoverSound;

extern unordered_map<string, string> translations;

extern string tr(const string& key);

bool no_name = false;
bool name_exists = false;
bool open_new_project = false;

static ImGuiID some_hovered = 0;
static ImGuiID some_was_hovered = 0;

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
    
    ImGui::Text(tr("menu.enter_project_name").c_str());
    ImGui::InputText("##InputName", inputBuffer, IM_ARRAYSIZE(inputBuffer));

    if (no_name) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), tr("menu.error_no_name").c_str());
    }

    if (name_exists) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), tr("menu.error_name_exists").c_str());
    }

    some_hovered = 0;
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button(tr("menu.back").c_str(), ImVec2(200, 70)))
    {
        ma_sound_start(&clickSound);
        no_name = false;
        name_exists = false;
        currentAppState = AppState::MainMenu;
    }
    if(ImGui::IsItemHovered())
    {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_sound_start(&hoverSound);
        some_hovered = id;
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button(tr("menu.create").c_str(), ImVec2(200, 70)))
    {
        ma_sound_start(&clickSound);
        if (inputBuffer[0] == '\0') {
            no_name = true;
        }
        else {
            no_name = false;
            string projectName = string(inputBuffer);
            string projectPath = "user_data/projects/" + projectName + ".txt";
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
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_sound_start(&hoverSound);
        some_hovered = id;
    }
    some_was_hovered = some_hovered;
    ImGui::End();
}

