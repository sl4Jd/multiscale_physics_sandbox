# =============================
if(NOT Python3_Interpreter_FOUND)
message(FATAL_ERROR "Python3 is required to generate glad sources. Please install Python3 or vendor pre-generated glad.")
endif()


# Prepare output directories
set(GLAD_GEN_DIR ${CMAKE_BINARY_DIR}/_glad_gen)
file(MAKE_DIRECTORY ${GLAD_GEN_DIR})


# Build the command-line args for glad
# Example: --api gl:core=4.6 --generator c --out-path <dir>
if(NOT GLAD_LANGUAGE)
set(GLAD_LANGUAGE c)
endif()


set(GLAD_ARGS --out-path ${GLAD_GEN_DIR} --generator ${GLAD_LANGUAGE})
foreach(api ${GLAD_API})
list(APPEND GLAD_ARGS --api ${api})
endforeach()
if(GLAD_EXTENSIONS)
list(APPEND GLAD_ARGS --extensions ${GLAD_EXTENSIONS})
endif()
list(APPEND GLAD_ARGS c)


add_custom_command(
OUTPUT ${GLAD_GEN_DIR}/src/gl.c ${GLAD_GEN_DIR}/include/glad/gl.h
COMMAND ${Python3_EXECUTABLE} -m glad ${GLAD_ARGS}
DEPENDS glad_repo
COMMENT "Generating GLAD loader ("${GLAD_API}")"
VERBATIM
)


add_library(${GLAD_OUT_TARGET} STATIC ${GLAD_GEN_DIR}/src/gl.c)
target_include_directories(${GLAD_OUT_TARGET} PUBLIC ${GLAD_GEN_DIR}/include)
endfunction()