cmake_minimum_required(VERSION 3.15)

project(MyProject LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output executable name
set(EXECUTABLE_NAME project)

# Add include directories
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


# Source files
set(SOURCES
    src/main.cpp
    src/main_menu.cpp
    src/open_project.cpp
    src/create_project.cpp
    src/settings.cpp
    src/game_engine.cpp
    extern_deps/imgui/image_implementation.cpp
    extern_deps/imgui/imgui.cpp
    extern_deps/imgui/imgui_widgets.cpp
    extern_deps/imgui/imgui_tables.cpp
    extern_deps/imgui/imgui_draw.cpp
    extern_deps/imgui/imgui_impl_opengl3.cpp
    extern_deps/imgui/imgui_impl_glfw.cpp
    extern_deps/imgui/imgui_demo.cpp
    extern_deps/glad/glad.c
    extern_deps/miniaudio/miniaudio.c
)

# Add the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/extern_deps
)

# Link librariess
target_link_libraries(${EXECUTABLE_NAME}
    glfw3
    assimp
    opengl32
)

# Windows-specific option (hide console)
if(WIN32)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -mwindows)
endif()

# Place executable in project root instead of build/
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
