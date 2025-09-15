cmake_minimum_required(VERSION 3.15)

project(MyProject LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output executable name
set(EXECUTABLE_NAME project)

# Add include directories
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
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
    glfw
<<<<<<< HEAD
    assimp
    opengl32
)

# Windows- option (hide console)
if(WIN32)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -mwindows)
endif()
=======
    assimp::assimp
    opengl32
)

# Putting needed directories in bin
set(RESOURCE_FOLDERS local fonts resources sound shaders projects)

foreach(folder IN LISTS RESOURCE_FOLDERS)
    add_custom_command(TARGET project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/${folder}
                $<TARGET_FILE_DIR:project>/${folder})
endforeach()
>>>>>>> 0053e8cf0764d2722f90b3858a38bae904b6933a

# Place executable in project root instead of build/
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
