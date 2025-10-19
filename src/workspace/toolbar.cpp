#include <imgui.h>
#include "toolbar.h"
#include "game_engine.h"
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;
using namespace std;

extern json general_settings;

extern float masterVolume, UIVolume;

void Toolbar(){
    ImGui::SetNextWindowBgAlpha(1.0f); 
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1400, 100), ImGuiCond_Always);
        ImGui::Begin("Tools", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings);
    
    if(ImGui::Button("Add object", ImVec2(0, 0))){
        ImGui::OpenPopup("add object");
    }
    ImGui::SameLine();
    if(ImGui::Button("Save", ImVec2(0, 0))){
        
    }
    ImGui::SameLine();
    if(ImGui::Button("Settings", ImVec2(0, 0))){
        ImGui::OpenPopup("settings_popup");
    }
    if(ImGui::BeginPopupModal("add object")) {
        ImGui::Text("Add object");
        if(ImGui::Button("cube", ImVec2(100,70))){
            
        }
        ImGui::SameLine();
        if(ImGui::Button("plane", ImVec2(100,70))){
            
        }
        if(ImGui::Button("cancel")){
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("add object")){
            
        }
        ImGui::EndPopup();
    }
    if(ImGui::BeginPopupModal("settings_popup")) {
        ImGui::SliderFloat("master_volume", &masterVolume, 0.0f, 1.0f);
        ImGui::SliderFloat("UI_volume", &UIVolume, 0.0f, 1.0f);
        if(ImGui::Button("cancel")){
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Save")) {
            general_settings["master_volume"] = masterVolume;
            general_settings["UI_volume"] = UIVolume;
            std::ofstream output("user_data/user_settings/settings.json");
            output << general_settings.dump(4);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::End();
    
}

