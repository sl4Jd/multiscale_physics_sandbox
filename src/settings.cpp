#include <imgui/imgui.h>
#include <miniaudio/miniaudio.h>
#include <iostream>
#include <string>

#include "main.h"
#include "appstate.h"

using namespace std;

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;

extern string tr(const string& key);

extern ma_engine engine;
extern const char* click_sound;
extern const char* hover_sound;

static ImGuiID some_hovered = 0;
static ImGuiID some_was_hovered = 0;

void Settings(){
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::Begin("Settings", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);
    some_hovered = 0;
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button(tr("menu.back").c_str(), ImVec2(200, 70))) {
        ma_engine_play_sound(&engine, click_sound, NULL);
        currentAppState = AppState::MainMenu;
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_engine_play_sound(&engine, hover_sound, NULL);
        some_hovered = id;
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button(tr("save").c_str(), ImVec2(200, 70))) {
        ma_engine_play_sound(&engine, click_sound, NULL);
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_engine_play_sound(&engine, hover_sound, NULL);
        some_hovered = id;
    }
    some_was_hovered = some_hovered;
    ImGui::End();
}