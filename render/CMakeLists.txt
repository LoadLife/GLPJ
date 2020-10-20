include_directories(${Third_Part}/include)
include_directories(${CMAKE_BINARY_DIR})
include_directories(include)
aux_source_directory(src SRC_FILE)
message(STATUS ${SRC_FILE})
add_executable(render ${SRC_FILE})
target_link_libraries(render ${Third_Part}/libs/OpenGl32.Lib
                             ${Third_Part}/libs/glew32s.lib
                             ${Third_Part}/libs/glfw3.lib
                             ${Third_Part}/libs/freetype.lib
)
