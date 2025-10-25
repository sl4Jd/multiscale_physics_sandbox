#include <imgui.h>
#include <string>
#include <vector>
#include "toolbar.h"
#include "game_engine.h"
#include <json.hpp>
#include <fstream>
#include "sounds.h"
#include "translatons.h"
#include "save_project.h"

using json = nlohmann::json;
using namespace std;

extern json general_settings;

extern float masterVolume, UIVolume;

extern bool stop_camera_movement;

extern string name_of_project;

static bool combo_open = false;

static bool combo_first_time = true;

static vector<string> languages;
static int current_lang_index = 0;
static string language = "";


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
        stop_camera_movement = true;
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
        SaveProject(name_of_project);
    }
    if (ImGui::IsItemHovered()) {
        ImGuiID id = ImGui::GetItemID();
        if(some_was_hovered != id) play_hover_sound();
        some_hovered = id;
    }
    ImGui::SameLine();
    if(ImGui::Button(translate("menu.settings").c_str(), ImVec2(0, 0))){
        play_click_sound();
        stop_camera_movement = true;
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
            stop_camera_movement = false;
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
    ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Always);
    if(ImGui::BeginPopupModal(translate("menu.settings").c_str())) {
        ImGui::Text(translate("menu.settings").c_str());
        ImGui::Separator();
        ImGui::Text((translate("language")+": ").c_str());
        ImGui::SameLine();
        if(combo_first_time) {
            languages = getAvailableLanguages();
            current_lang_index = getLanguageIndex();
            language = getCurrentLanguage();
            combo_first_time = false;
        }
        if (ImGui::BeginCombo("##LanguageCombo", language.c_str())) {
        if(combo_open == false) {
            combo_open = true;
            play_click_sound();
        }
        for (int n = 0; n < languages.size(); n++) {
            bool is_selected = (current_lang_index == n);
            if (ImGui::Selectable(languages[n].c_str(), is_selected, ImGuiSelectableFlags_None, ImVec2(0, 0))) {
                play_click_sound();
                current_lang_index = n;
                language = languages[current_lang_index];
            }
            if (ImGui::IsItemHovered()) {
                ImGuiID id = ImGui::GetItemID();
                if(some_was_hovered != id) play_hover_sound();
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
                if(some_was_hovered != id) play_hover_sound();
                some_hovered = id;
            }
        }
        ImGui::SliderFloat(translate("master_volume").c_str(), &masterVolume, 0.0f, 1.0f);
        ImGui::SliderFloat(translate("UI_volume").c_str(), &UIVolume, 0.0f, 1.0f);
        if(ImGui::Button(translate("cancel").c_str())){
            play_click_sound();
            stop_camera_movement = false;
            combo_first_time = true;
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
            general_settings["language"] = language;
            std::ofstream output("user_data/user_settings/settings.json");
            output << general_settings.dump(4);
            stop_camera_movement = false;
            loadLanguage(language);
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

