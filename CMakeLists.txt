cmake_minimum_required(VERSION 3.15)

project(MyProject LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output executable name
set(EXECUTABLE_NAME multi-scale-sandbox)

# Add include directories
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)


# Source files
set(SOURCES
    src/main.cpp
    src/main_menu.cpp
    src/open_project.cpp
    src/create_project.cpp
    src/settings.cpp
    src/game_engine.cpp
    src/zip_utils.cpp
    vendored_extern_deps/imgui/image_implementation.cpp
    vendored_extern_deps/imgui/imgui.cpp
    vendored_extern_deps/imgui/imgui_widgets.cpp
    vendored_extern_deps/imgui/imgui_tables.cpp
    vendored_extern_deps/imgui/imgui_draw.cpp
    vendored_extern_deps/imgui/imgui_impl_opengl3.cpp
    vendored_extern_deps/imgui/imgui_impl_glfw.cpp
    vendored_extern_deps/imgui/imgui_demo.cpp
    vendored_extern_deps/glad/glad.c
    vendored_extern_deps/miniaudio/miniaudio.c
    vendored_extern_deps/miniz/miniz.c
)

# Add the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/vendored_extern_deps
    ${CMAKE_SOURCE_DIR}/src/includes
)

# Link librariess
target_link_libraries(${EXECUTABLE_NAME}
    glfw
    assimp::assimp
    OpenGL::GL
)

# Windows- option (hide console)
if(WIN32)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -mwindows)
endif()

add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/user_data
)

# Place executable in build/bin instead of project root
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
