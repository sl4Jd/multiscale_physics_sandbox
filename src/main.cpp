#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <miniaudio/miniaudio.h>
#include <json.hpp>
#include <fstream>

#include "game_engine.h"
#include "create_project.h"
#include "main.h"
#include "appstate.h"
#include "open_project.h"
#include "main_menu.h"
#include "settings.h"

int screenWidth;
int screenHeight;
int windowWidth;
int windowHeight;

const char* click_sound = "sound/click.wav";
const char* hover_sound = "sound/hover.wav";

ma_sound clickSound;
ma_sound hoverSound;
extern bool open_new_project;
extern bool open_project;

ImFont* lucida_big;
ImFont* lucida_small;

AppState currentAppState = AppState::MainMenu;

GLFWwindow* main_window;

static ma_decoder decoder; 
static ma_device device;
ma_engine engine;

using json = nlohmann::json;
using namespace std;

unordered_map<string, string> translations;
string currentLang = "english";

bool loadLanguage(const string& lang) {
    ifstream file("local/" + lang + ".json");
    if (!file.is_open()) return false;

    json j;
    file >> j;

    translations.clear();
    for (auto it = j.begin(); it != j.end(); ++it) {
        translations[it.key()] = it.value();
    }
    currentLang = lang;
    return true;
}
string tr(const std::string& key) {
    if (translations.count(key)) return translations[key];
    return key; // fallback
}
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    // Read PCM frames from the decoder and put them into the output buffer
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput; // Prevent unused variable warning
}

// load device and decoder(not needed for basic effects)
void load_audio(){
    const char* filename = "sound/I_Wonder.mp3"; 
    if(ma_decoder_init_file(filename, NULL, &decoder) != MA_SUCCESS) {
        printf("Failed to load audio file: %s\n", filename);
        return;
    }

    // Configure playback device
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    // Initialize playback device
    if(ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to initialize playback device.\n");
        ma_decoder_uninit(&decoder);
        return;
    }

    // Start playback
    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return;
    }
}

int main()
{
    // Setup GLFW
    if (!glfwInit())
        return 1;

    // Setup OpenGL version (3.0+)
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    screenWidth = videoMode->width;
    screenHeight = videoMode->height;
    // Create window with graphics context
    windowWidth = screenWidth / 2;
    windowHeight = screenHeight / 2;
    main_window = glfwCreateWindow(windowWidth, windowHeight, "engine", NULL, NULL);
    if (main_window == NULL)
        return 1;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    glfwSetWindowPos(main_window, posX, posY);
    glfwMakeContextCurrent(main_window);
    glfwSwapInterval(1); // enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImFontGlyphRangesBuilder builder;
    builder.AddText("đčšćžĐČŠĆŽ");           // add the characters you need
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // add default ASCII

    ImVector<ImWchar> glyph_ranges;
    builder.BuildRanges(&glyph_ranges);
    lucida_big = io.Fonts->AddFontFromFileTTF("fonts/lucon.ttf", 32.0f, NULL, glyph_ranges.Data);
    lucida_small = io.Fonts->AddFontFromFileTTF("fonts/lucon.ttf", 16.0f, NULL, glyph_ranges.Data);
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(main_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    loadLanguage(currentLang);
    // not needed for basic effects
    //load_audio();
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        printf("Failed to init engine\n");
        return -1;
    }
    ma_sound_init_from_file(&engine, "sound/click.wav", 0, NULL, NULL, &clickSound);
    ma_sound_init_from_file(&engine, "sound/hover.wav", 0, NULL, NULL, &hoverSound);
    // Main loop
    while (!glfwWindowShouldClose(main_window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Our Start Window UI
        if(currentAppState == AppState::MainMenu) {
            ShowStartWindow();
        } else if (currentAppState == AppState::CreateProject) {
            CreateProject();
        }
        else if (currentAppState == AppState::OpenProject) {
            OpenProject();
        }
        else if (currentAppState == AppState::Settings) {
            Settings();
        }
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(main_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(main_window);
    }
    
    glfwDestroyWindow(main_window);
    if(open_new_project) {
        StartNewProject();
    }
    if(open_project){
        StartNewProject();
    }
    // Cleanup
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    ma_engine_uninit(&engine);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
