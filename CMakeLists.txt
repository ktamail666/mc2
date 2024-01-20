cmake_minimum_required (VERSION 3.10)

# Find mold executable
find_program(MOLD_EXECUTABLE NAMES mold)
if(MOLD_EXECUTABLE)
    set(CMAKE_LINKER ${MOLD_EXECUTABLE})
endif()

find_program(CLANG_COMPILER clang-${CLANG_VERSION})
find_program(CLANG_COMPILER_CXX clang++-${CLANG_VERSION})
if(CLANG_COMPILER)
    set(CMAKE_C_COMPILER ${CLANG_COMPILER})
endif()
if(CLANG_COMPILER_CXX)
    set(CMAKE_CXX_COMPILER ${CLANG_COMPILER_CXX})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LIBCPP_NO_ABI_TAG -stdlib=libc++ --rtlib=compiler-rt")
    set(CLANG_VERSION 19)
endif()
project(mc2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CMakeToolsHelpers OPTIONAL)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(0)
        option(USE_CLANG_TIDY "help" OFF)

        if(UNIX AND USE_CLANG_TIDY)
                find_program(TIDY NAMES clang-tidy)
                if(NOT TIDY)
                        message(FATAL_ERROR "clang tidy not found!")
                endif()
                set(CMAKE_CXX_CLANG_TIDY ${TIDY}
                        "--extra-arg-before=-std=c++20"
                        "--extra-arg-before=-I${CMAKE_CURRENT_BINARY_DIR}/gen")
        endif()

        add_compile_options(
                -Werror=array-bounds
                -Wnon-virtual-dtor
                -Woverloaded-virtual
                -Wcast-align
                -Wshadow
                -Wmissing-field-initializers
                -Wunused-function
                -Wunused-variable
                -Wunused-value
                -Wunused-label
                -Wunused
                -Wunused-macros
                -Wunused-parameter

                -Wno-unknown-pragmas
                -fno-strict-aliasing

                -fvisibility=hidden

                -Wdouble-promotion
                -Wnull-dereference
                -Wmisleading-indentation
                -Wformat=2
                -Wsuggest-override
                -Wconversion
                )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(MSVC)
	add_definitions(-DPLATFORM_WINDOWS)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	message("Compiling on M$ Windows")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64-v3 -Wno-unused-local-typedefs -Werror=array-bounds -Wno-switch -Wwrite-strings -Wunused-variable -fpermissive")
    # -Wno-unused-local-typedefs - to disable "typedef was ignored on this declaration"
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_ARMOR -D_DEBUG -DBUGLOG -DLAB_ONLY")
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /MP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()


add_definitions(-DLINUX_BUILD)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

if(NOT (TARGET SDL2_mixer OR TARGET SDL2_mixer::SDL2_mixer))
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_mixer REQUIRED SDL2_mixer IMPORTED_TARGET)
    add_library(SDL2_mixer::SDL2_mixer ALIAS PkgConfig::SDL2_mixer)
endif()

#required by text_tool
find_package(SDL2_ttf)
if(NOT (TARGET SDL2_ttf OR TARGET SDL2_ttf::SDL2_ttf-static))
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_ttf REQUIRED SDL2_ttf IMPORTED_TARGET)
    add_library(SDL2_ttf::SDL2_ttf ALIAS PkgConfig::SDL2_ttf)
endif()

find_package(ZLIB REQUIRED)

message("Found SDL2 package ${SDL_LIBS}" )
message("SDL2 prefix: ${SDL2_PREFIX}")
message("Zlib library found in ${ZLIB}")

get_filename_component(COM_PATH1 "GameOS/include" ABSOLUTE)
get_filename_component(COM_PATH2 "GameOS/gameos" ABSOLUTE)

set(COMMON_INCLUDE_DIRS  ${COM_PATH1} ${COM_PATH2})

add_subdirectory("mclib/" "out/mclib")
add_subdirectory("mclib/mlr" "out/mclib/mlr")
add_subdirectory("mclib/gosfx" "out/mclib/gosfx")
add_subdirectory("mclib/stuff" "out/mclib/stuff")
add_subdirectory("gui" "out/gui")
add_subdirectory("GameOS/gameos" "out/GameOS/gameos")
add_subdirectory("GameOS/src" "out/windows")

set(SOURCES
    "code/mechcmd2.cpp"
    "code/artlry.cpp"
    "code/bldng.cpp"
    "code/carnage.cpp"
    "code/collsn.cpp"
    "code/comndr.cpp"
    "code/contact.cpp"
    "code/gameobj.cpp"
    "code/gate.cpp"
    "code/goal.cpp"
    "code/group.cpp"
    "code/gvehicl.cpp"
    "code/light.cpp"
    "code/mech.cpp"
    "code/mover.cpp"
    "code/objmgr.cpp"
    "code/objtype.cpp"
    "code/tacordr.cpp"
    "code/team.cpp"
    "code/terrobj.cpp"
    "code/turret.cpp"
    "code/warrior.cpp"
    "code/weaponbolt.cpp"
    "code/ablmc2.cpp"
    "code/gamecam.cpp"
    "code/mission.cpp"
    "code/mission2.cpp"
    "code/objective.cpp"
    "code/saveload.cpp"
    "code/trigger.cpp"
    "code/weather.cpp"
    "gui/aanimobject.cpp"
    "gui/aedit.cpp"
    "code/attributemeter.cpp"
    "code/controlgui.cpp"
    "code/forcegroupbar.cpp"
    "code/gametacmap.cpp"
    "code/infowindow.cpp"
    "code/keyboardref.cpp"
    "code/mechicon.cpp"
    "code/missiongui.cpp"
    "code/mpconnectiontype.cpp"
    "code/mpgamebrowser.cpp"
    "code/mphostgame.cpp"
    "code/mploadmap.cpp"
    "code/mpparameterscreen.cpp"
    "code/mpsetuparea.cpp"
    "code/optionsarea.cpp"
    "code/optionsscreenwrapper.cpp"
    "code/pausewindow.cpp"
    "code/prefs.cpp"
    "code/simplecamera.cpp"
    "code/componentlistbox.cpp"
    "code/loadscreen.cpp"
    "code/logistics.cpp"
    "code/logisticscomponent.cpp"
    "code/logisticsdata.cpp"
    "code/logisticsdialog.cpp"
    "code/logisticsmech.cpp"
    "code/logisticsmechdisplay.cpp"
    "code/logisticsmechicon.cpp"
    "code/logisticsmissioninfo.cpp"
    "code/logisticspilot.cpp"
    "code/logisticspilotlistbox.cpp"
    "gui/logisticsscreen.cpp"
    "code/logisticsvariant.cpp"
    "code/mainmenu.cpp"
    "code/mechbayscreen.cpp"
    "code/mechlabscreen.cpp"
    "code/mechlistbox.cpp"
    "code/mechlopedia.cpp"
    "code/mechpurchasescreen.cpp"
    "code/missionbegin.cpp"
    "code/missionbriefingscreen.cpp"
    "code/missionresults.cpp"
    "code/missionselectionscreen.cpp"
    "code/mpprefs.cpp"
    "code/pilotreadyscreen.cpp"
    "code/pilotreviewarea.cpp"
    "code/salvagemecharea.cpp"
    "code/simplecomponentlistbox.cpp"
    "code/gamesound.cpp"
    "code/radio.cpp"
    "code/movemgr.cpp"
    "code/chatwindow.cpp"
    "code/mpstats.cpp"
    "code/multplyr.cpp"
    "code/mc2movie.cpp"
    )

include_directories(${COMMON_INCLUDE_DIRS}
        "mclib"
        "gui"
        "code"
        "netlib"
)

if(UNIX)
	set(ADDITIONAL_LIBS dl)
	message("Not win32 system")
else()
	# winmm for timeGetTime, maybe switch to GetTickCount to remove this dependency
	set(ADDITIONAL_LIBS winmm)
endif()

add_executable(mc2 ${SOURCES} ${MAIN_SRC})
target_link_libraries(mc2 PRIVATE
    mclib
    gosfx
    mlr
    stuff
    gui
    gameos
    gameos_main
    windows
    ZLIB::ZLIB
    SDL2::SDL2main
    SDL2_mixer::SDL2_mixer
    ${ADDITIONAL_LIBS}
)

add_subdirectory(res out/res)
add_subdirectory(data_tools out/data_tools)
add_subdirectory(text_tool out/text_tool)
add_subdirectory(Viewer out/Viewer)

if(UNIX) # TODO temporarily disable on Windows
    add_custom_target(mc2_copy_data ALL
        #COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build_scripts
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data_scripts ${CMAKE_SOURCE_DIR}/build_scripts
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:mc2>/shaders
    )

    file(GLOB files "${CMAKE_SOURCE_DIR}/build_scripts/*.fst" "${CMAKE_SOURCE_DIR}/build_scripts/*.cfg" "${CMAKE_SOURCE_DIR}/build_scripts/testtxm.tga")
    foreach(file ${files})
        get_filename_component(FILENAME_WO_NAME ${file} NAME_WE)
        get_filename_component(FILENAME_NAME ${file} NAME)
        add_custom_target(mc2_create_symlinks_${FILENAME_WO_NAME} ALL
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${file} $<TARGET_FILE_DIR:mc2>/${FILENAME_NAME}
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${file} $<TARGET_FILE_DIR:mc2>/${FILENAME_NAME}
        )
    endforeach()

    add_custom_target(mc2_create_symlinks2 ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:mc2>/shaders
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/build_scripts/data $<TARGET_FILE_DIR:mc2>/data
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/build_scripts/assets $<TARGET_FILE_DIR:mc2>/assets
    )
endif()

# TODO Debian should create data deb
# TODO currently data installed under /usr/bin/...
# TODO there is many issues with DEB package generation

set(CPACK_PACKAGE_NAME "mc2")
set(CPACK_PACKAGE_VENDOR "mc2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "mc2")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_DEBIAN_PACKAGE_DEPENDS libsdl2-dev libsdl2-mixer-dev) # TODO check all dependencies
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "https://github.com/ktamail666/mc2")
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "DEB")
endif()

include(CPack)

install(TARGETS mc2 mc2res DESTINATION .)
install(DIRECTORY shaders DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/mc2srcdata/build_scripts/data DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/mc2srcdata/build_scripts/assets DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/mc2srcdata/build_scripts/ DESTINATION .) # TODO iterate over only fst and cfg
