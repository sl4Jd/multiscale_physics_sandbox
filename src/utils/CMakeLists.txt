

# sources
set(UTIL_SOURCES
    zip_utils.cpp
    sounds.cpp
)

# make library
add_library(utils STATIC ${UTIL_SOURCES})