#include <imgui.h>
#include "toolbar.h"


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

    }
    ImGui::SameLine();
    if(ImGui::Button("Save", ImVec2(0, 0))){

    }
    ImGui::SameLine();
    if(ImGui::Button("Settings", ImVec2(0, 0))){

    }
    ImGui::End();
    
}

