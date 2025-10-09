#include <imgui.h>
#include "appstate.h"
#include "game_engine.h"
#include "main.h"
#include "create_project.h"
#include "zip_utils.h"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <miniaudio.h>

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
            string projectPath = "user_data/projects/" + projectName;
            if(filesystem::exists(projectPath)) {
                name_exists = true;
            }
            else {
                name_exists = false;

                //Create directory and scene.json file
                projectPath = "user_data/projects/working";
                filesystem::create_directory(projectPath);
                filesystem::path file_path = filesystem::path(projectPath) / "scene.json";

                // Open the file
                ofstream ofs(file_path);
                if (!ofs) {
                    cerr << "Failed to create file: " << file_path << "\n";
                }

                //Write in scene.json
                ofs << "{\n";
                ofs << "  \"name\": \"" << projectName << "\",\n";
                ofs << "  \"objects\": [\n";
                ofs << "    { \"name\": \"object1\", \"type\": \"model\", \"shape\": \"backpack\", \"posx\": 10.0, \"posy\": 0.0, \"posz\": 0.0,  \"scalex\": 0.33, \"scaley\": 0.33, \"scalez\": 0.33},\n";
                ofs << "    { \"name\": \"object2\", \"type\": \"primitive\", \"shape\": \"cube\",  \"posx\": -1.0, \"posy\": 0.0, \"posz\": -1.0,  \"scalex\": 1.0, \"scaley\": 1.0, \"scalez\": 1.0},\n";
                ofs << "    { \"name\": \"object3\", \"type\": \"primitive\", \"shape\": \"cube\",  \"posx\": 2.0, \"posy\": 0.0, \"posz\": 0.0,  \"scalex\": 1.0, \"scaley\": 1.0, \"scalez\": 1.0}\n";
                ofs << "    { \"name\": \"object4\", \"type\": \"primitive\", \"shape\": \"plane\",  \"posx\": 0.0, \"posy\": 0.0, \"posz\": 0.0,  \"scalex\": 1.0, \"scaley\": 1.0, \"scalez\": 1.0}\n";
                ofs << "  ]\n";
                ofs << "}\n";
                
                ofs.close();
                
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

