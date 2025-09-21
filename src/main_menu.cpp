#include <imgui/imgui.h>
#include <miniaudio/miniaudio.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <json.hpp>
#include <fstream>
#include <vector>
#include "appstate.h"
#include "main.h"

using json = nlohmann::json;
using namespace std;

extern AppState currentAppState;

extern bool loadLanguage(const string& lang);
extern string tr(const string& key);

extern int windowWidth;
extern int windowHeight;

extern ImFont* lucida_big;
extern ImFont* lucida_small;

extern ma_sound clickSound;
extern ma_sound hoverSound;

static ImGuiID some_hovered = 0;
static ImGuiID some_was_hovered = 0;
static bool combo_open = false;

extern json settings;

extern vector<const char*> languages;
extern int current_lang_index;

static void ShowLanguageSelector() {
    ImGui::PushFont(lucida_small);
    ImGui::SetCursorPosX(windowWidth/8*7);
    ImGui::SetCursorPosY(100);
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##LanguageCombo", languages[current_lang_index])) {
        if(combo_open == false) {
            combo_open = true;
            ma_sound_start(&clickSound);
        }
        for (int n = 0; n < languages.size(); n++) {
            bool is_selected = (current_lang_index == n);
            if (ImGui::Selectable(languages[n], is_selected, ImGuiSelectableFlags_None, ImVec2(0, 0))) {
                ma_sound_start(&clickSound);
                current_lang_index = n; // update selected language
                settings["language"] = languages[current_lang_index];
                ofstream file("user_data/user_settings/settings.json");
                file << settings.dump(3);
                loadLanguage(languages[current_lang_index]);
            }
            if (ImGui::IsItemHovered()) {
                ImGuiID id = ImGui::GetItemID();
                if(some_was_hovered != id) ma_sound_start(&hoverSound);
                some_hovered = id;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus(); // highlight the current choice
        }
        ImGui::EndCombo();
    }
    else {
        combo_open = false;
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_hovered == 0) {
            if(some_was_hovered != id) ma_sound_start(&hoverSound);
            some_hovered = id;
        }
    }
    ImGui::PopItemWidth();
    ImGui::PopFont();
}
void ShowStartWindow() {
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin("InvisibleWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SetCursorPosY(50);
    ImGui::SetCursorPosX(windowWidth/2 - 250);
    ImGui::Text("Game Engine version 0.1.5");
    some_hovered = 0;
    ShowLanguageSelector();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetCursorPosX(windowWidth/2 - 250);
    if (ImGui::Button(tr("menu.new_project").c_str(), ImVec2(500, 100))) {
        currentAppState = AppState::CreateProject;
        ma_sound_start(&clickSound);
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_sound_start(&hoverSound);
        some_hovered = id;
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX(windowWidth/2 - 250);
    if (ImGui::Button(tr("menu.open_project").c_str(), ImVec2(500, 100))) {
        currentAppState = AppState::OpenProject;
        ma_sound_start(&clickSound);
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_sound_start(&hoverSound);
        some_hovered = id;
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX(windowWidth/2 - 250);
    if (ImGui::Button((tr("menu.settings")).c_str(), ImVec2(500, 100))) {
        currentAppState = AppState::Settings;
        ma_sound_start(&clickSound);
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) ma_sound_start(&hoverSound);
        some_hovered = id;
    }

    some_was_hovered = some_hovered;
    ImGui::End();
}