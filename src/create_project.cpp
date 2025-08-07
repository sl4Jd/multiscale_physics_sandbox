#include "includes/imgui.h"
#include "appstate.h"
#include "game_engine.h"

static char inputBuffer[256] = "";

extern AppState currentAppState;

extern int windowWidth;
extern int windowHeight;

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
        

    ImGui::Text("Project Name:");
    ImGui::InputText("##InputName", inputBuffer, IM_ARRAYSIZE(inputBuffer));

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Back", ImVec2(200, 70)))
    {
        currentAppState = AppState::MainMenu;
    }

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 100);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 250); 

    if (ImGui::Button("Create", ImVec2(200, 70)))
    {
        StartNewProject();
    }
    ImGui::End();
}

