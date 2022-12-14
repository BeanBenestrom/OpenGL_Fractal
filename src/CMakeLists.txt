file(GLOB source_files ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(Fractal ${source_files})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set_target_properties(Fractal PROPERTIES DEBUG_POSTFIX "_Debug")
target_link_libraries(Fractal PRIVATE lib)


function(copy_to)
    list(GET ARGN 0 dir)
    list(POP_FRONT ARGN)

    foreach(file ${ARGN})
        cmake_path(GET file FILENAME name)
        configure_file(${file} ${dir}/${name} COPYONLY)
    endforeach()  
endfunction()

file(GLOB shaders ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*)
file(GLOB images ${CMAKE_CURRENT_SOURCE_DIR}/textures/*)
# file(fractal_settings ${CMAKE_CURRENT_SOURCE_DIR}/textures/*)

copy_to(${CMAKE_CURRENT_BINARY_DIR}/shaders ${shaders})
copy_to(${CMAKE_CURRENT_BINARY_DIR}/textures ${images})
copy_to(${CMAKE_CURRENT_BINARY_DIR} "Fractal_Settings.vs")



if (CMAKE_BUILD_TYPE STREQUAL "")
    # Build type is not set eg. command was "cmake .."
    message(STATUS "  Diag: Build type was unspecified, set to Release")
    set(CMAKE_BUILD_TYPE Release)
else ()
    message(STATUS "  Diag: Build type specified as '${CMAKE_BUILD_TYPE}'")
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "_DEBUG")
else ()
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "_RELEASE")
endif ()