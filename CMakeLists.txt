cmake_minimum_required(VERSION 4.0)
include_directories(src)

project(myengine)
# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

# Create your game executable target as usual
add_executable(myengine WIN32 tests.cpp)

# Link to the actual SDL3 library.
target_link_libraries(myengine PRIVATE SDL3::SDL3)


if(WIN32)
    add_custom_command(
        TARGET myengine POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:myengine>
        VERBATIM
    )
    
endif()