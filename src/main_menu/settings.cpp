#include <imgui.h>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include "main.h"
#include "appstate.h"
#include "settings.h"
#include "sounds.h"
#include "translatons.h"

using json = nlohmann::json;
using namespace std;

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;

extern json settings;

static ImGuiID some_hovered = 0;
static ImGuiID some_was_hovered = 0;

extern float master_volume;
extern float ui_volume;

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
    ImGui::SetCursorPosY(50);
    ImGui::SetCursorPosX(50);
    ImGui::SliderFloat(translate("master_volume").c_str(), &master_volume, 0.0f, 1.0f);
    ImGui::SliderFloat(translate("UI_volume").c_str(), &ui_volume, 0.0f, 1.0f);
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button(translate("menu.back").c_str(), ImVec2(200, 70))) {
        play_click_sound();
        currentAppState = AppState::MainMenu;
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button(translate("save").c_str(), ImVec2(200, 70))) {
        play_click_sound();
        set_click_sound(master_volume, ui_volume);
        set_hover_sound(master_volume, ui_volume);
        settings["master_volume"] = master_volume;
        settings["UI_volume"] = ui_volume;
        ofstream output("user_data/user_settings/settings.json");
        output << settings.dump(4);
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    some_was_hovered = some_hovered;
    ImGui::End();
}