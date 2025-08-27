cmake_minimum_required(VERSION 3.15)
project(MyProject LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output executable name
set(EXECUTABLE_NAME project)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Source files
set(SOURCES
    src/main.cpp
    src/main_menu.cpp
    src/open_project.cpp
    src/create_project.cpp
    src/settings.cpp
    src/game_engine.cpp
    include/image_implementation.cpp
    include/imgui/imgui.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_impl_opengl3.cpp
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_demo.cpp
    include/glad.c
    include/miniaudio/miniaudio.c
)

# Add the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Link directories (for external libs in lib/)
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Link libraries
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
