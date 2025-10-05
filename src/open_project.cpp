#include "imgui/imgui.h"
#include "appstate.h"
#include "game_engine.h"
#include "main.h"
#include "open_project.h"
#include "zip_utils.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "miniaudio/miniaudio.h"

using namespace std;

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;

extern ma_sound clickSound;
extern ma_sound hoverSound;

extern unordered_map<string, string> translations;
extern string tr(const string& key);

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
static bool hovered_inside_select = false;

vector<string> labels;
int counts;

void save_edit(){
    already_named = false;
    if(strlen(editBuffer) == 0) {
        name_is_empty = true;
        justActivated = true;
    }
    else{
        name_is_empty = false;
        for(int j = 0; j < counts; j++) {
            if(labels[j] == editBuffer && j != selectedIndex) {
                already_named = true;
                justActivated = true;
            }
        }
        if(!already_named) {
            filesystem::rename("user_data/projects/" + labels[selectedIndex] + ".msps", "user_data/projects/" + string(editBuffer) + ".msps");
            labels[selectedIndex] = editBuffer;
            editing = false;
            name_is_empty = false;
        }
    }
}
void load_project_files(){
    std::string folderPath = "user_data/projects";
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
    counts = labels.size();
    load_projects = true;
}
void DrawSelectableBoxes()
{
    if(!load_projects){
        load_project_files();
    }
    for (int i = 0; i < counts; i++)
    {
        ImVec2 itemSize(0, 80);
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
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), tr("menu.error_no_name").c_str());
            }
            if(already_named){
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), tr("menu.error_name_exists").c_str());
            }
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0,0,0,0));
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));
            bool was_selected = ImGui::Selectable(labels[i].c_str(), selectedIndex == i, 0, itemSize);
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            if (was_selected)
            {
                ma_sound_start(&clickSound);
                if(editing){
                    save_edit();
                }
                else{
                    selectedIndex = i;
                }
            }
            if(ImGui::IsItemHovered())
            {
                if(!hovered_inside_select){
                ImGuiID id = ImGui::GetItemID();
                if(some_was_hovered != id) ma_sound_start(&hoverSound);
                some_hovered = id;
                }
            }
            if(selectedIndex == i && !editing)
            {
                hovered_inside_select = false;
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(255, 255, 0, 255), 4.0f, 0, 5.0f);
                ImVec2 pos = ImGui::GetCursorPos(); 
                pos.y -= 50;
                pos.x = max.x - 350;              
                ImGui::SetCursorPos(pos);
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 255, 0, 255));
                if(ImGui::Button(tr("delete").c_str())) {
                    ma_sound_start(&clickSound);
                    ImGui::OpenPopup(tr("menu.delete_popup").c_str());
                }
                if (ImGui::IsItemHovered())
                {
                    ImGuiID id = ImGui::GetItemID();
                    if(some_was_hovered != id) ma_sound_start(&hoverSound);
                    some_hovered = id;
                    hovered_inside_select = true;
                }
                ImGui::SameLine();
                if (ImGui::Button(tr("rename").c_str()))
                {
                    ma_sound_start(&clickSound);
                    editing = true;
                    justActivated = true;
                    strncpy(editBuffer, labels[i].c_str(), sizeof(editBuffer) - 1);
                    editBuffer[sizeof(editBuffer) - 1] = '\0';
                }
                if (ImGui::IsItemHovered())
                {
                    ImGuiID id = ImGui::GetItemID();
                    if(some_was_hovered != id) ma_sound_start(&hoverSound);
                    some_hovered = id;
                    hovered_inside_select = true;
                }
                ImGui::PopStyleColor();
            }
        }
        ImGui::Dummy(ImVec2(0, 15));
    }
    if (ImGui::BeginPopupModal(tr("menu.delete_popup").c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(tr("delete.confirm").c_str(), labels[selectedIndex].c_str());
        if (ImGui::Button(tr("no").c_str()))
        {
            ma_sound_start(&clickSound);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button(tr("yes").c_str()))
        {
            ma_sound_start(&clickSound);
            filesystem::remove("user_data/projects/" + labels[selectedIndex] + ".msps");
            labels.erase(labels.begin() + selectedIndex);
            counts--;
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
    ImGui::Text(tr("menu.select_project").c_str());
    some_hovered = 0;
    DrawSelectableBoxes();
    if (no_selected) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), tr("menu.select_project").c_str());
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button(tr("menu.back").c_str(), ImVec2(200, 70)))
    {
        ma_sound_start(&clickSound);
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
        if(some_was_hovered != id) ma_sound_start(&hoverSound);
        some_hovered = id;
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button(tr("menu.open").c_str(), ImVec2(200, 70)))
    {
        ma_sound_start(&clickSound);
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
                try {
                    UnzipFile(labels[selectedIndex]);
                }
                catch (const runtime_error& e){
                    cerr << "Error: " << e.what() << "\n";
                }
                open_project = true;

            }
        }
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_sound_start(&hoverSound);
        some_hovered = id;
    }
    some_was_hovered = some_hovered;
    ImGui::End();
} 

