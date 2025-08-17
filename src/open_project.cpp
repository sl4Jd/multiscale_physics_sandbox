#include "includes/imgui.h"
#include "appstate.h"
#include "game_engine.h"
#include "main_menu.h"

#include <GLFW/glfw3.h>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

static char inputBuffer[256] = "";

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;

bool no_selected = false;
bool open_project = false;
bool load_projects = false;
bool editing = false;
static char editBuffer[128] = "";
static int selectedIndex = -1;


using namespace std;

vector<string> labels;
int count;
void save_edit(){
    filesystem::rename("projects/" + labels[selectedIndex], "projects/" + string(editBuffer));
    labels[selectedIndex] = editBuffer;
    editing = false;
}
void load_project_files(){
    std::string folderPath = "projects";
    try {
        for (const auto& entry : filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                labels.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    count = labels.size();
    load_projects = true;
}
void DrawSelectableBoxes()
{
    if(!load_projects){
        load_project_files();
    }
    for (int i = 0; i < count; i++)
    {
        ImVec2 itemSize(300, 80);
        if(editing && selectedIndex == i) {
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText("##edit", editBuffer, sizeof(editBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                save_edit();
            }
            
        }
        else {
            if (ImGui::Selectable(labels[i].c_str(), selectedIndex == i, 0, itemSize))
            {
                if(editing){
                    save_edit();
                }
                else{
                    selectedIndex = i;
                }
            }
            if(selectedIndex == i && !editing)
            {
                ImGui::SameLine();
                if(ImGui::Button("Delete", ImVec2(150, 40))) {
                    filesystem::remove("projects/" + labels[i]);
                    labels.erase(labels.begin() + i);
                    count--;
                    i--;
                    selectedIndex = -1;
                }
                ImGui::SameLine();
                if (ImGui::Button("Rename", ImVec2(150, 40)))
                {
                    editing = true;
                    strncpy(editBuffer, labels[i].c_str(), sizeof(editBuffer) - 1);
                    editBuffer[sizeof(editBuffer) - 1] = '\0';
                }
            
            }
        }
    }
}
void OpenProject()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::Begin("Open Project", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Select a project");
    DrawSelectableBoxes();
    if (no_selected) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Select a project!");
    }

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Back", ImVec2(200, 70)))
    {
        if(editing){
            save_edit();
        }
        else {
            no_selected = false;
            selectedIndex = -1;
            currentAppState = AppState::MainMenu;
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button("Create", ImVec2(200, 70)))
    {
        if(editing){
            save_edit();
        }
        else {
            if (selectedIndex == -1) {
                no_selected = true;
            }
            else {
                no_selected = false;
                glfwSetWindowShouldClose(main_window, true);
                open_project = true;

            }
        }
    }
    ImGui::End();
}

