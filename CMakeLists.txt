cmake_minimum_required(VERSION 3.13)
project(FedoraFrisbee CXX)

if(NOT EMSCRIPTEN)
    message(FATAL_ERROR
        "This CMakeLists.txt is the WebAssembly build only. "
        "For the Windows desktop build, use GenerateProject.bat (premake5/VS2017). "
        "To build for the web: emcmake cmake -B build-wasm && emmake make -C build-wasm")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Source)

file(GLOB_RECURSE SOURCES
    ${SRC_ROOT}/*.cpp
    ${SRC_ROOT}/*.h
)

# stb_image lives under Source/Vendor and is compiled inline.
add_executable(FedoraFrisbee ${SOURCES})

target_include_directories(FedoraFrisbee PRIVATE
    ${SRC_ROOT}
    ${SRC_ROOT}/FedGame
    ${SRC_ROOT}/PCH
    ${SRC_ROOT}/Utility
    ${SRC_ROOT}/Vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Lib/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Lib/GLM
    # Stub directory: <GL/glew.h> resolves here on wasm. Must be listed before
    # any directory that might shadow it (none in the wasm build, but be safe).
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/wasm-stubs
    # GLFW, freetype, GLES3 headers come from Emscripten's USE_PORT bundles.
)

# Every TU under Source/ explicitly includes FedPCH.h, and Source/PCH is on the
# include path above, so no force-include is needed (premake's pchheader is a
# VS-only PCH optimization, not a force-include).

# Emscripten ports: GLFW3 + freetype.
# WebGL2 + FULL_ES3 lets the existing GLES3-style code link unchanged.
# Assets are mounted in MEMFS at the literal virtual paths the source already
# requests ("../Assets/..." and "Shaders/..."), avoiding any code changes.
set(EM_LINK_FLAGS
    "-O3"
    "-flto"
    "-sUSE_GLFW=3"
    "-sUSE_FREETYPE=1"
    "-sUSE_WEBGL2=1"
    "-sFULL_ES3=1"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sASSERTIONS=0"
    "-sALLOW_MEMORY_GROWTH=0"
    "-sINITIAL_MEMORY=256MB"
    "-sSTACK_SIZE=4MB"
    "-sEXPORTED_FUNCTIONS=['_main','_FedoraFrisbeeSendEscape','_FedoraFrisbeeProfilerGetMetric']"
    "-sEXPORTED_RUNTIME_METHODS=['callMain','ccall','FS']"
    "-sINVOKE_RUN=0"
    "-sEXIT_RUNTIME=0"
    "-sNO_DISABLE_EXCEPTION_CATCHING"
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/web/index.html"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/Assets@../Assets"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/Source/Shaders@/Shaders"
    "--use-preload-plugins"
)
set(EM_COMPILE_FLAGS
    "-O3"
    "-flto"
    "-sUSE_GLFW=3"
    "-sUSE_FREETYPE=1"
    "-sUSE_WEBGL2=1"
    "-sFULL_ES3=1"
)

target_compile_options(FedoraFrisbee PRIVATE ${EM_COMPILE_FLAGS})
target_link_options(FedoraFrisbee PRIVATE ${EM_LINK_FLAGS})

# Tell Emscripten to emit an HTML wrapper using our shell file.
set_target_properties(FedoraFrisbee PROPERTIES
    SUFFIX ".html"
    OUTPUT_NAME "FedoraFrisbee"
)

# Build assimp from source as a static lib (Emscripten port doesn't ship one).
# Disable everything except OBJ to keep the WASM binary small.
include(FetchContent)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(assimp)
target_link_libraries(FedoraFrisbee PRIVATE assimp)

message(STATUS "FedoraFrisbee → WebAssembly build configured.")
message(STATUS "Run: emmake make -j  (output: FedoraFrisbee.{html,js,wasm,data})")
