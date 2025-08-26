cmake_minimum_required(VERSION 3.14)
project(MyApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4    # or latest stable version
)
FetchContent_MakeAvailable(glfw)

# Your executable
add_executable(my_app main.cpp)

# Link against glfw
target_link_libraries(my_app PRIVATE glfw)
