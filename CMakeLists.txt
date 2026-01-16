cmake_minimum_required(VERSION 3.20)
project(myengine)

set(CMAKE_CXX_STANDARD 17)

# --- 1. DEFINE ASSETS ---
# Add your model files here so they get copied too!
set(ASSET_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/vert.spv" 
    "${CMAKE_CURRENT_SOURCE_DIR}/frag.spv"
    "${CMAKE_CURRENT_SOURCE_DIR}/Untitled.glb"  # <--- Make sure you have this file!
    "${CMAKE_CURRENT_SOURCE_DIR}/Audi R8.glb"  # <--- Make sure you have this file!
)

# --- 2. DEPENDENCIES SETUP ---
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "~/SDL/SDL/sdl_install")
elseif (WIN32)
    # Point to the folder containing SDL3Config.cmake
    set(SDL3_DIR "C:/SDL/cmake") 
endif ()

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)

# --- 3. FETCH LIBRARIES (GLM & CGLTF) ---
include(FetchContent)

# A. GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3
)
FetchContent_MakeAvailable(glm)

# B. CGLTF (Model Loader)
FetchContent_Declare(
    cgltf
    GIT_REPOSITORY https://github.com/jkuhlmann/cgltf.git
    GIT_TAG        v1.15
)
FetchContent_MakeAvailable(cgltf)

# --- 4. BUILD TARGET ---
add_executable(myengine src/volk.c tests.cpp)

target_include_directories(myengine PRIVATE ${Vulkan_INCLUDE_DIRS} "src")

# Link everything together
target_link_libraries(myengine PRIVATE 
    SDL3::SDL3-shared 
    Vulkan::Headers
    glm::glm
)

# --- 5. ASSET HANDLING ---
target_sources(myengine PRIVATE ${ASSET_FILES})

if(APPLE)
    # macOS: Copy into Bundle Resources
    set_source_files_properties(${ASSET_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    
    set_target_properties(myengine PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.myengine.test"
        RESOURCE "${ASSET_FILES}"
    )
else()
    # Windows/Linux: Copy next to executable
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

# --- 6. WINDOWS DLL COPYING ---
if(WIN32)
    add_custom_command(
        TARGET myengine POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
        $<TARGET_FILE:SDL3::SDL3-shared> 
        $<TARGET_FILE_DIR:myengine>
        VERBATIM
    )
endif()