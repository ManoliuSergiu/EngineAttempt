cmake_minimum_required(VERSION 3.20) # Note: VERSION 4.0 doesn't exist yet! 3.20 is standard.
set(CMAKE_CXX_STANDARD 17)
project(myengine)

set(SHADER_FILES 
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert"
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag"
    )


if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "~/SDL/SDL/sdl_install")
elseif (WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/SDL")
endif ()
# 1. Setup Options
option(MYGAME_VENDORED "Use vendored libraries" OFF)
# 2. Find Dependencies (Must happen before creating targets)
find_package(Vulkan REQUIRED)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # If this fails, we need to point CMake to your SDL3 folder (see below)
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)
endif()

# 3. Create Targets
# We use one target name 'myengine' and adjust properties based on OS
add_executable(myengine tests.cpp src/volk.c
        tests.cpp)

# 4. Target Properties & Includes (Target must exist now!)
target_include_directories(myengine PRIVATE 
    ${Vulkan_INCLUDE_DIRS} 
    "src"
)

# 5. Link Libraries
target_link_libraries(myengine PRIVATE 
    SDL3::SDL3-shared 
    Vulkan::Headers
)
target_sources(myengine PRIVATE ${SHADER_FILES})
set_source_files_properties(${SHADER_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/shaders)

# 6. OS Specific Logic
if(WIN32)
    # Copy DLL to exe folder so it runs
    add_custom_command(
        TARGET myengine POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:myengine>
        VERBATIM
    )
endif()

if(APPLE)
    set_target_properties(myengine PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.myengine.test"
    )
endif()
