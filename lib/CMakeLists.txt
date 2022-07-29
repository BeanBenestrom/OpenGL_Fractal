add_library(lib glad.c stb_image.cpp)

target_link_libraries(lib "${CMAKE_CURRENT_SOURCE_DIR}/libglfw3.a")
target_include_directories(lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")