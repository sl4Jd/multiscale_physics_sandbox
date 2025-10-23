#include <imgui.h>
#include "toolbar.h"
#include "game_engine.h"
#include <json.hpp>
#include <fstream>
#include "sounds.h"

using json = nlohmann::json;
using namespace std;

extern json general_settings;

extern float masterVolume, UIVolume;

static ImGuiID some_hovered = 0;
static ImGuiID some_was_hovered = 0;

void Toolbar(){
    ImGui::SetNextWindowBgAlpha(1.0f); 
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1400, 100), ImGuiCond_Always);
        ImGui::Begin("Tools", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings);

    some_hovered = 0;
    
    if(ImGui::Button("Add object", ImVec2(0, 0))){
        play_click_sound();
        ImGui::OpenPopup("add object");
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    ImGui::SameLine();
    if(ImGui::Button("Save", ImVec2(0, 0))){
        play_click_sound();
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    ImGui::SameLine();
    if(ImGui::Button("Settings", ImVec2(0, 0))){
        play_click_sound();
        ImGui::OpenPopup("settings_popup");
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    if(ImGui::BeginPopupModal("add object")) {
        ImGui::Text("Add object");
        if(ImGui::Button("cube", ImVec2(100,70))){
            play_click_sound();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::SameLine();
        if(ImGui::Button("plane", ImVec2(100,70))){
            play_click_sound();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        if(ImGui::Button("cancel")){
            play_click_sound();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::SameLine();
        if(ImGui::Button("add object")){
            play_click_sound();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::EndPopup();
    }
    if(ImGui::BeginPopupModal("settings_popup")) {
        ImGui::SliderFloat("master_volume", &masterVolume, 0.0f, 1.0f);
        ImGui::SliderFloat("UI_volume", &UIVolume, 0.0f, 1.0f);
        if(ImGui::Button("cancel")){
            play_click_sound();
            ImGui::CloseCurrentPopup();
        }
        if(ImGui::IsItemHovered()){
            play_hover_sound();
        }
        ImGui::SameLine();
        if(ImGui::Button("Save")) {
            play_click_sound();
            general_settings["master_volume"] = masterVolume;
            general_settings["UI_volume"] = UIVolume;
            std::ofstream output("user_data/user_settings/settings.json");
            output << general_settings.dump(4);
            ImGui::CloseCurrentPopup();
        }
        if(ImGui::IsItemHovered()){
            play_hover_sound();
        }
        ImGui::EndPopup();
    }
    some_was_hovered = some_hovered;
    ImGui::End();
    
}

