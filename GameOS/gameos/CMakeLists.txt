cmake_minimum_required (VERSION 3.16)
project(gameos)

add_library(gameos
    gameos.cpp
    gameos_graphics.cpp
    gameos_res.cpp
    gameos_fileio.cpp
    gameos_input.cpp
    gameos_debugging.cpp
    gameos_sound.cpp
    gos_render.cpp
    gos_font.cpp
    gos_input.cpp

    utils/stream.cpp
    utils/camera.cpp
    utils/gl_utils.cpp
    utils/gl_render_constants.cpp
    utils/Image.cpp
    utils/logging.cpp
    utils/shader_builder.cpp
    utils/matrix.cpp
    utils/vec.cpp
    utils/timing.cpp
    utils/string_utils.cpp
    utils/file_utils.cpp
)
target_include_directories(gameos PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gameos PUBLIC
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    GLEW::GLEW
    )


add_library(gameos_main gameosmain.cpp)
target_include_directories(gameos_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gameos_main PUBLIC gameos)
