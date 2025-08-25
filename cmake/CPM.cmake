# =============================
# cmake/CPM.cmake (bootstrapper)
# =============================
# Create this file at: cmake/CPM.cmake
# It downloads CPM.cmake once into the build folder and includes it
set(CPM_DOWNLOAD_VERSION 0.38.7)
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
file(DOWNLOAD
"https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
"${CMAKE_BINARY_DIR}/cmake/CPM.cmake"
TLS_VERIFY ON
)
endif()
include("${CMAKE_BINARY_DIR}/cmake/CPM.cmake")