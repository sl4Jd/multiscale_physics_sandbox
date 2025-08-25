# =============================
# Root CMakeLists.txt
# =============================
cmake_minimum_required(VERSION 3.20)
project(MyApp LANGUAGES C CXX)


# Options you can toggle
option(USE_GTK "Enable GTK integration (Linux only)" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries where applicable" OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Where to keep helper cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


# --- CPM: dependency manager powered by CMake FetchContent ---
include(cmake/CPM.cmake)


# ===============
# Dependencies
# ===============


# GLFW
CPMAddPackage(
NAME glfw
GITHUB_REPOSITORY glfw/glfw
GIT_TAG 3.4
OPTIONS
"GLFW_BUILD_EXAMPLES OFF"
"GLFW_BUILD_TESTS OFF"
"GLFW_BUILD_DOCS OFF"
)


# GLAD (generated at configure-time via Python) — OpenGL 4.6 Core
# If Python is present, generate sources; otherwise, fail with a friendly message
include(cmake/glad_generate.cmake)
my_generate_glad(
OUT_TARGET glad
API gl:core=4.6
LANGUAGE c
EXTENSIONS ""
)

# ImGui (ocornut/imgui) + backends for GLFW + OpenGL3
include(cmake/imgui.cmake)
add_imgui_library(
TARGET imgui
WITH_BACKENDS glfw opengl3
)


# miniaudio (header-only)
CPMAddPackage(
NAME miniaudio
GITHUB_REPOSITORY mackron/miniaudio
GIT_TAG 0.11.17
)


# Assimp
CPMAddPackage(
NAME assimp
GITHUB_REPOSITORY assimp/assimp
GIT_TAG v5.4.3
OPTIONS
"ASSIMP_BUILD_TESTS OFF"
"ASSIMP_BUILD_ASSIMP_TOOLS OFF"
"ASSIMP_BUILD_SAMPLES OFF"
)


# GLM (header-only)
CPMAddPackage(
NAME glm
GITHUB_REPOSITORY g-truc/glm
GIT_TAG 1.0.1
)


# nlohmann/json (header-only)
CPMAddPackage(
NAME nlohmann_json
GITHUB_REPOSITORY nlohmann/json
GIT_TAG v3.11.3
)

# GTK (optional; Linux only, via pkg-config)
find_package(PkgConfig REQUIRED)
# Prefer GTK4; fall back to GTK3
pkg_check_modules(GTK4 QUIET IMPORTED_TARGET gtk4)
if(GTK4_FOUND)
set(GTK_FOUND TRUE)
set(GTK_TARGET PkgConfig::GTK4)
else()
pkg_check_modules(GTK3 QUIET IMPORTED_TARGET gtk+-3.0)
if(GTK3_FOUND)
set(GTK_FOUND TRUE)
set(GTK_TARGET PkgConfig::GTK3)
endif()
endif()
if(NOT GTK_FOUND)
message(FATAL_ERROR "USE_GTK=ON but neither GTK4 nor GTK3 were found via pkg-config.")
endif()
endif()


# =============================
# Your executable
# =============================


# Gather your sources (adjust as needed)
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
src/*.cpp src/*.c
)


add_executable(${PROJECT_NAME}
${APP_SOURCES}
)


# Include directories for header-only deps
if(miniaudio_ADDED)
target_include_directories(${PROJECT_NAME} PRIVATE ${miniaudio_SOURCE_DIR})
endif()
if(glm_ADDED)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
endif()
if(nlohmann_json_ADDED)
# nlohmann/json exports a target too; include dir for header-only usage is optional
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
endif()


# Link libraries
# - glad provides OpenGL loader
# - glfw is the windowing system
# - imgui is the immediate-mode GUI
# - assimp for model loading
# - GTK if enabled

# On Linux, also link dl and pthread if needed
set(PLATFORM_LIBS dl pthread)
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE
glad
glfw
imgui
assimp
${PLATFORM_LIBS}
)


# GTK target if requested
if(USE_GTK AND GTK_FOUND)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK_TARGET})
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_GTK)
endif()


# Enable warnings (tweak for MSVC vs GCC/Clang)
if(MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


# On Windows, ensure we use UTF-8 source/exec char set
if(MSVC)
add_definitions(-DUNICODE -D_UNICODE)
endif()


# =============================
# Post-build notes
# =============================
# - You may need system packages for GLFW/Assimp on Linux (dev headers); CPM builds from source, but X11/Wayland and OpenGL headers are required.
# - For GLFW on Linux, consider installing: xorg-dev, libwayland-dev, libxkbcommon-dev, etc.
# - For Assimp on Windows, CPM builds it with MSVC.