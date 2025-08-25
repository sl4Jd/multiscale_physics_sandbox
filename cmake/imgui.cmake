# =============================
# cmake/imgui.cmake — builds ImGui core + backends as a static lib
# =============================
# Create this file at: cmake/imgui.cmake
function(add_imgui_library)
set(options)
set(oneValueArgs TARGET)
set(multiValueArgs WITH_BACKENDS)
cmake_parse_arguments(IMGUI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})


if(NOT IMGUI_TARGET)
message(FATAL_ERROR "add_imgui_library: please specify TARGET <name>")
endif()


CPMAddPackage(
NAME imgui_src
GITHUB_REPOSITORY ocornut/imgui
GIT_TAG v1.91.0
)


set(IMGUI_CORE_SOURCES
${imgui_src_SOURCE_DIR}/imgui.cpp
${imgui_src_SOURCE_DIR}/imgui_draw.cpp
${imgui_src_SOURCE_DIR}/imgui_widgets.cpp
${imgui_src_SOURCE_DIR}/imgui_tables.cpp
)


# Backends
set(BACKEND_SOURCES)
foreach(backend IN LISTS IMGUI_WITH_BACKENDS)
if(backend STREQUAL "glfw")
list(APPEND BACKEND_SOURCES ${imgui_src_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
elseif(backend STREQUAL "opengl3")
list(APPEND BACKEND_SOURCES ${imgui_src_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
elseif(backend STREQUAL "sdl2")
list(APPEND BACKEND_SOURCES ${imgui_src_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp)
endif()
endforeach()


add_library(${IMGUI_TARGET} STATIC ${IMGUI_CORE_SOURCES} ${BACKEND_SOURCES})
target_include_directories(${IMGUI_TARGET} PUBLIC
${imgui_src_SOURCE_DIR}
${imgui_src_SOURCE_DIR}/backends
)
# ImGui backend needs the GLFW headers when using the GLFW backend
if("glfw" IN_LIST IMGUI_WITH_BACKENDS)
target_link_libraries(${IMGUI_TARGET} PUBLIC glfw)
endif()
endfunction()