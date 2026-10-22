cmake_minimum_required(VERSION 3.24)
project(jrpg LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Para que clang-tidy / IntelliSense sepan cómo se compila cada archivo
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Todo estático: un único .exe, sin DLLs que copiar
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# --- SDL3: ventana, render, teclado, sonido ---
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.8
  GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(SDL3)

# --- SDL3_image: cargar PNG (sprites, tilesets) ---
# Usamos el decodificador interno (stb_image): PNG/JPG/BMP sin dependencias externas
set(SDLIMAGE_VENDORED     OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_BACKEND_STB  ON  CACHE BOOL "" FORCE)
set(SDLIMAGE_AVIF         OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL          OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIF          OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP         OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_SAMPLES      OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS        OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_INSTALL      OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG        release-3.4.8
  GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(SDL3_image)

# --- El juego ---
# Cada .cpp nuevo del proyecto se añade a esta lista (los .h no hace falta)
add_executable(jrpg
  src/main.cpp
  src/logger.cpp
  src/personaje.cpp
  src/habilidad.cpp
  src/combate.cpp
  src/seleccionPersonaje.cpp
  src/interfaz.cpp
  src/controladorPJ.cpp
  src/controladorIA.cpp
  src/tipo.cpp
)
target_link_libraries(jrpg PRIVATE SDL3_image::SDL3_image SDL3::SDL3)

# Avisos del compilador al nivel alto: te ayudan a encontrar bugs pronto
if(MSVC)
  target_compile_options(jrpg PRIVATE /W4 /utf-8)
else()
  target_compile_options(jrpg PRIVATE -Wall -Wextra)
endif()

# Copiar la carpeta assets/ junto al .exe después de cada compilación
add_custom_command(TARGET jrpg POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:jrpg>/assets)
