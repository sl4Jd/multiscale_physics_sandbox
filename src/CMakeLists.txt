
set(EXECUTABLE_NAME multi-scale-sandbox)

# Add the executable
set(SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_menu/main_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/zip_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/workspace/game_engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/workspace/save_project.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_menu/open_project.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_menu/create_project.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_menu/settings.cpp
)
# Export to parent scope
set(SOURCES ${SOURCES} PARENT_SCOPE)
