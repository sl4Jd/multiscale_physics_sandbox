#include <imgui.h>
#include <string>
#include "toolbar.h"
#include "game_engine.h"
#include <json.hpp>
#include <fstream>
#include "sounds.h"
#include "translatons.h"

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
    
    if(ImGui::Button(translate("add_object").c_str(), ImVec2(0, 0))){
        play_click_sound();
        ImGui::OpenPopup(translate("add_object").c_str());
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    ImGui::SameLine();
    if(ImGui::Button(translate("save").c_str(), ImVec2(0, 0))){
        play_click_sound();
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    ImGui::SameLine();
    if(ImGui::Button(translate("menu.settings").c_str(), ImVec2(0, 0))){
        play_click_sound();
        ImGui::OpenPopup(translate("menu.settings").c_str());
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    if(ImGui::BeginPopupModal(translate("add_object").c_str())) {
        ImGui::Text(translate("add_object").c_str());
        if(ImGui::Button(translate("cube").c_str(), ImVec2(100,70))){
            play_click_sound();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::SameLine();
        if(ImGui::Button(translate("plane").c_str(), ImVec2(100,70))){
            play_click_sound();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        if(ImGui::Button(translate("cancel").c_str())){
            play_click_sound();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::SameLine();
        if(ImGui::Button(translate("add_object").c_str())){
            play_click_sound();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::EndPopup();
    }
    if(ImGui::BeginPopupModal(translate("menu.settings").c_str())) {
        ImGui::SliderFloat(translate("master_volume").c_str(), &masterVolume, 0.0f, 1.0f);
        ImGui::SliderFloat(translate("UI_volume").c_str(), &UIVolume, 0.0f, 1.0f);
        if(ImGui::Button(translate("cancel").c_str())){
            play_click_sound();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::SameLine();
        if(ImGui::Button(translate("save").c_str())) {
            play_click_sound();
            set_click_sound(masterVolume, UIVolume);
            set_hover_sound(masterVolume, UIVolume);    
            general_settings["master_volume"] = masterVolume;
            general_settings["UI_volume"] = UIVolume;
            std::ofstream output("user_data/user_settings/settings.json");
            output << general_settings.dump(4);
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsItemHovered()) {
            ImGuiID id = ImGui::GetItemID();
            if(some_was_hovered != id) play_hover_sound();
            some_hovered = id;
        }
        ImGui::EndPopup();
    }
    some_was_hovered = some_hovered;
    ImGui::End();
    
}

