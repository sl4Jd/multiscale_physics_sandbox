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
add_library(imgui STATIC
    vendored_extern_deps/imgui/imgui.cpp
    vendored_extern_deps/imgui/imgui_widgets.cpp
    vendored_extern_deps/imgui/imgui_tables.cpp
    vendored_extern_deps/imgui/imgui_draw.cpp
    vendored_extern_deps/imgui/imgui_impl_opengl3.cpp
    vendored_extern_deps/imgui/imgui_impl_glfw.cpp
    vendored_extern_deps/imgui/imgui_demo.cpp
    vendored_extern_deps/imgui/image_implementation.cpp
)
target_include_directories(imgui PUBLIC vendored_extern_deps/imgui)
target_link_libraries(imgui PRIVATE glfw)

add_library(glad STATIC vendored_extern_deps/glad/glad.c)
target_include_directories(glad PUBLIC vendored_extern_deps/glad)

add_library(miniaudio STATIC vendored_extern_deps/miniaudio/miniaudio.c)
target_include_directories(miniaudio PUBLIC vendored_extern_deps/miniaudio)

add_library(miniz STATIC vendored_extern_deps/miniz/miniz.c)
target_include_directories(miniz PUBLIC vendored_extern_deps/miniz)


add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE
    vendored_extern_deps/json
)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    vendored_extern_deps/glm
)

add_subdirectory(src)

add_executable(${EXECUTABLE_NAME} src/main.cpp)

# Link libraries
target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
    imgui 
    glad 
    miniaudio 
    miniz 
    nlohmann_json 
    glfw 
    main_menu
    workspace
    utils
)

# add include directories for the executable
target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
)

# add include directories and libraries for main_menu
target_include_directories(main_menu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
)
target_link_libraries(main_menu PRIVATE
    imgui 
    miniaudio 
    nlohmann_json 
    glfw 
)

# add include directories and libraries for workspace
target_include_directories(workspace PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
)
target_link_libraries(workspace PRIVATE
    imgui 
    miniaudio 
    nlohmann_json 
    glfw 
    glad
    glm
    OpenGL::GL
    assimp::assimp
)

# add include directories and libraries for utils
target_include_directories(utils PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes
)
target_link_libraries(utils PRIVATE
    miniz
)


# Windows- option (hide console)
if(WIN32)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -mwindows)
endif()


# transport assets and user_data folders to the bin directory
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/user_data $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/user_data
)

# Place executable in build/bin instead of project root
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
