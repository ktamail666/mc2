
add_executable(text_tool main.cpp)

target_compile_definitions(text_tool PRIVATE USE_ASSEMBLER_CODE=0 LINUX_BUILD)

target_link_libraries(text_tool
    gameos
    windows
    SDL2::SDL2main
    SDL2_ttf::SDL2_ttf
    GLEW::GLEW
    ${ADDITIONAL_LIBS}
    OpenGL::GL
)
