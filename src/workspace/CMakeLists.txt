

# sources
set(WORKSPACE_SOURCES
    game_engine.cpp
    save_project.cpp
    primitives.cpp
)

# make library
add_library(workspace ${WORKSPACE_SOURCES})