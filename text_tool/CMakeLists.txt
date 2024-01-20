add_executable(text_tool main.cpp)

target_compile_definitions(text_tool PRIVATE
        LINUX_BUILD
)

target_link_libraries(text_tool
    gameos
    windows
    SDL2::SDL2main
    SDL2_ttf::SDL2_ttf
    ${ADDITIONAL_LIBS}
)
