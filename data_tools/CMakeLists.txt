cmake_minimum_required (VERSION 3.19)

add_library(tools_common INTERFACE)
target_compile_definitions(tools_common INTERFACE DISABLE_GAMEOS_MAIN)

add_executable(makefst makefst.cpp)
target_link_libraries(makefst PUBLIC
    mclib
    gameos
    stuff
    windows
    tools_common
    ZLIB::ZLIB
    SDL2::SDL2main
    ${ADDITIONAL_LIBS})

add_executable(pak pak.cpp common.hpp)
target_link_libraries(pak
    mclib
    stuff
    tools_common
    gameos
    windows
    ZLIB::ZLIB
    SDL2::SDL2main
    ${ADDITIONAL_LIBS})

add_executable(aseconv aseconv.cpp common.hpp)
target_link_libraries(aseconv
    mclib
    gosfx
    mlr
    tools_common
    gameos
    stuff
    windows
    ZLIB::ZLIB
    SDL2::SDL2main
    ${SDL2_mixer}
    ${ADDITIONAL_LIBS}
)

add_executable(makersp makersp.cpp)
target_link_libraries(makersp
    tools_common
    mclib
    stuff
    gameos
    windows
    ZLIB::ZLIB
    SDL2::SDL2main
    ${ADDITIONAL_LIBS})
