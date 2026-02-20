cmake_minimum_required(VERSION 3.15)
project(SI3LN VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Emscripten produces .js + .wasm (+ .data for preloaded assets)
set(CMAKE_EXECUTABLE_SUFFIX ".js")

# Sources: APIClient_wasm.cpp replaces APIClient.cpp / libcurl
set(SOURCES
    ${GAME_SRC}/src/main.cpp
    ${GAME_SRC}/src/Game.cpp
    ${GAME_SRC}/src/Player.cpp
    ${GAME_SRC}/src/Enemy.cpp
    ${GAME_SRC}/src/Bullet.cpp
    ${GAME_SRC}/src/GameState.cpp
    ${GAME_SRC}/src/Stubs.cpp
    ${GAME_SRC}/src/UI/UIStubs.cpp
    ${GAME_SRC}/src/APIClient_wasm.cpp
)

# nlohmann/json installed via apt-get (nlohmann-json3-dev) in Dockerfile
# Header is at /usr/include/nlohmann/json.hpp -- no FetchContent needed.

include_directories(
    ${GAME_SRC}/include
    ${GAME_SRC}/include/UI
    /usr/include          # nlohmann/json.hpp (emcc does not search /usr/include by default)
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Compile flags (SDL2 ports must be set at compile AND link time)
target_compile_options(${PROJECT_NAME} PRIVATE
    -O2
    -sUSE_SDL=2
    -sUSE_SDL_IMAGE=2
    "-sSDL2_IMAGE_FORMATS=[png,jpg]"
    -sUSE_SDL_TTF=2
    -sUSE_SDL_MIXER=2
)

# Link flags
set(WASM_ASSETS_DIR "${GAME_SRC}/assets_web")

target_link_options(${PROJECT_NAME} PRIVATE
    -sUSE_SDL=2
    -sUSE_SDL_IMAGE=2
    "-sSDL2_IMAGE_FORMATS=[png,jpg]"
    -sUSE_SDL_TTF=2
    -sUSE_SDL_MIXER=2
    -sASYNCIFY=1
    -sASYNCIFY_STACK_SIZE=65536
    -sALLOW_MEMORY_GROWTH=1
    -sINITIAL_MEMORY=67108864
    -sFORCE_FILESYSTEM=1
    "--preload-file=${WASM_ASSETS_DIR}@/assets"
    "-sEXPORTED_RUNTIME_METHODS=[ccall,cwrap,malloc,free,UTF8ToString,stringToUTF8,lengthBytesUTF8]"
    "-sEXPORTED_FUNCTIONS=[_main,_malloc,_free]"
    -sENVIRONMENT=web
    -sMODULARIZE=1
    "-sEXPORT_NAME=SI3LN"
)
