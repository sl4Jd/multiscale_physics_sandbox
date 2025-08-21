#include <imgui/imgui.h>
#include "appstate.h"
#include "game_engine.h"
#include "main_menu.h"

#include <GLFW/glfw3.h>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <miniaudio/miniaudio.h>

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;
extern ma_engine engine;
extern const char* click_sound;
extern const char* hover_sound;

bool no_selected = false;
bool name_is_empty = false;
bool open_project = false;
bool load_projects = false;
bool delete_window = false;
bool already_named = false;
bool editing = false;
static char editBuffer[32] = "";
static int selectedIndex = -1;
static bool justActivated = false;

static ImGuiID some_hovered = 0;
static ImGuiID some_was_hovered = 0;

using namespace std;

vector<string> labels;
int count;

void save_edit(){
    already_named = false;
    if(strlen(editBuffer) == 0) {
        name_is_empty = true;
        justActivated = true;
    }
    else{
        name_is_empty = false;
        for(int j = 0; j < count; j++) {
            if(labels[j] == editBuffer && j != selectedIndex) {
                already_named = true;
                justActivated = true;
            }
        }
        if(!already_named) {
            filesystem::rename("projects/" + labels[selectedIndex] + ".txt", "projects/" + string(editBuffer) + ".txt");
            labels[selectedIndex] = editBuffer;
            editing = false;
            name_is_empty = false;
        }
    }
}
void load_project_files(){
    std::string folderPath = "projects";
    try {
        for (const auto& entry : filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                labels.push_back(entry.path().stem().string());
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
            if (justActivated) {
                ImGui::SetKeyboardFocusHere();
                justActivated = false;
            }
            if (ImGui::InputText("##edit", editBuffer, sizeof(editBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                save_edit();
            }
            if(name_is_empty){
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Project must have a name!");
            }
            if(already_named){
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Project name already exists!");
            }
        }
        else {
            if (ImGui::Selectable(labels[i].c_str(), selectedIndex == i, 0, itemSize))
            {
                ma_engine_play_sound(&engine, click_sound, NULL);
                if(editing){
                    save_edit();
                }
                else{
                    selectedIndex = i;
                }
            }
            if(ImGui::IsItemHovered())
            {
                ImGuiID id = ImGui::GetItemID();
                if(some_was_hovered != id) ma_engine_play_sound(&engine, hover_sound, NULL);
                some_hovered = id;
            }
            if(selectedIndex == i && !editing)
            {
                ImGui::SameLine();
                if(ImGui::Button("Delete", ImVec2(150, 40))) {
                    ma_engine_play_sound(&engine, click_sound, NULL);
                    ImGui::OpenPopup("Delete Confirmation");
                }
                ImGui::SameLine();
                if (ImGui::Button("Rename", ImVec2(150, 40)))
                {
                    ma_engine_play_sound(&engine, click_sound, NULL);
                    editing = true;
                    justActivated = true;
                    strncpy(editBuffer, labels[i].c_str(), sizeof(editBuffer) - 1);
                    editBuffer[sizeof(editBuffer) - 1] = '\0';
                }
                if (ImGui::IsItemHovered())
                {
                    ImGuiID id = ImGui::GetItemID();
                    if(some_was_hovered != id) ma_engine_play_sound(&engine, hover_sound, NULL);
                    some_hovered = id;
                }
            }
        }
    }
    if (ImGui::BeginPopupModal("Delete Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to delete the project        \n '%s'?", labels[selectedIndex].c_str());
        if (ImGui::Button("No", ImVec2(150, 40)))
        {
            ma_engine_play_sound(&engine, click_sound, NULL);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Yes", ImVec2(150, 40)))
        {
            ma_engine_play_sound(&engine, click_sound, NULL);
            filesystem::remove("projects/" + labels[selectedIndex] + ".txt");
            labels.erase(labels.begin() + selectedIndex);
            count--;
            selectedIndex = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
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
    some_hovered = 0;
    DrawSelectableBoxes();
    if (no_selected) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Select a project!");
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Back", ImVec2(200, 70)))
    {
        ma_engine_play_sound(&engine, click_sound, NULL);
        if(editing){
            save_edit();
        }
        else {
            no_selected = false;
            selectedIndex = -1;
            currentAppState = AppState::MainMenu;
        }
    }
    if (ImGui::IsItemHovered()) {
         ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_engine_play_sound(&engine, hover_sound, NULL);
        some_hovered = id;
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button("Load", ImVec2(200, 70)))
    {
        ma_engine_play_sound(&engine, click_sound, NULL);
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
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_engine_play_sound(&engine, hover_sound, NULL);
        some_hovered = id;
    }
    some_was_hovered = some_hovered;
    ImGui::End();
}

