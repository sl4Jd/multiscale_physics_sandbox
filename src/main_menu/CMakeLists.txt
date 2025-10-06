

#sources
set(MAIN_MENU_SOURCES
    main_menu.cpp
    create_project.cpp
    open_project.cpp
    settings.cpp
)

# make library
add_library(main_menu STATIC ${MAIN_MENU_SOURCES})