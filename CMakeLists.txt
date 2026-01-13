cmake_minimum_required(VERSION 3.20)
project(myengine)

set(CMAKE_CXX_STANDARD 17)

# --- 1. DEFINE ASSETS ---
# List all files you need at runtime (Shaders, Textures, etc.)
set(ASSET_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/vert.spv" 
    "${CMAKE_CURRENT_SOURCE_DIR}/frag.spv"
)

# --- 2. DEPENDENCIES ---
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "~/SDL/SDL/sdl_install")
elseif (WIN32)
    set(SDL3_DIR "C:/SDL/cmake")
endif ()




find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3)

# --- 3. BUILD TARGET ---
add_executable(myengine src/volk.c tests.cpp)

target_include_directories(myengine PRIVATE ${Vulkan_INCLUDE_DIRS} "src")
target_link_libraries(myengine PRIVATE SDL3::SDL3-shared Vulkan::Headers)

# --- 4. ASSET HANDLING (The Magic Part) ---

# A. Attach files to target (Makes them show up in IDE)
target_sources(myengine PRIVATE ${ASSET_FILES})

if(APPLE)
    # macOS: Mark them as Resources to go into the Bundle
    set_source_files_properties(${ASSET_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    
    set_target_properties(myengine PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.myengine.test"
        RESOURCE "${ASSET_FILES}"
    )
else()
    # Windows/Linux: Copy files to the build folder next to the .exe
    foreach(ASSET ${ASSET_FILES})
        add_custom_command(
            TARGET myengine POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${ASSET}"
            "$<TARGET_FILE_DIR:myengine>"
            VERBATIM
        )
    endforeach()
endif()
include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3  # Replace with the version you want
)
FetchContent_MakeAvailable(glm)
target_link_libraries(myengine PRIVATE glm::glm)
# --- 5. WINDOWS DLL COPYING ---
if(WIN32)
    # Auto-copy SDL3.dll to the exe folder so you don't get "DLL Missing" errors
    add_custom_command(
        TARGET myengine POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
        $<TARGET_FILE:SDL3::SDL3-shared> 
        $<TARGET_FILE_DIR:myengine>
        VERBATIM
    )
endif()