cmake_minimum_required(VERSION 3.24)
project(jrpg LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Enlace estático: un único .exe
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# --- SDL3 ---
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.8
  GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(SDL3)

# --- SDL3_image (PNG) ---
# Decodificador interno (stb_image)
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

# --- SDL3_ttf (texto) ---
# FreeType incluido; sin HarfBuzz ni PlutoSVG
set(SDLTTF_VENDORED  ON  CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ  OFF CACHE BOOL "" FORCE)
set(SDLTTF_PLUTOSVG  OFF CACHE BOOL "" FORCE)
set(SDLTTF_SAMPLES   OFF CACHE BOOL "" FORCE)
set(SDLTTF_INSTALL   OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG        release-3.2.2
  GIT_SUBMODULES external/freetype)
FetchContent_MakeAvailable(SDL3_ttf)

# --- Lógica del juego (sin SDL), compartida con las pruebas ---
add_library(jrpg_logica STATIC
  src/logger.cpp
  src/tipo.cpp
  src/habilidad.cpp
  src/personaje.cpp
  src/combate.cpp
  src/seleccionPersonaje.cpp
  src/controladorPJ.cpp
  src/controladorIA.cpp
  src/catalogo.cpp
)
target_include_directories(jrpg_logica PUBLIC src)

# --- Juego ---
add_executable(jrpg
  src/main.cpp
  src/interfaz.cpp
)
target_link_libraries(jrpg PRIVATE jrpg_logica SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)

# --- Pruebas (doctest) ---
# doctest se usa solo como cabecera
FetchContent_Declare(doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG        v2.4.12
  GIT_SHALLOW    TRUE
  SOURCE_SUBDIR  no-cmake)
FetchContent_MakeAvailable(doctest)

enable_testing()
add_executable(pruebas
  tests/main_pruebas.cpp
  tests/test_tipo.cpp
  tests/test_habilidad.cpp
  tests/test_personaje.cpp
  tests/test_controladorIA.cpp
  tests/test_controladorPJ.cpp
  tests/test_combate.cpp
  tests/test_catalogo.cpp
  tests/test_seleccionPersonaje.cpp
)
target_include_directories(pruebas PRIVATE ${doctest_SOURCE_DIR})
target_link_libraries(pruebas PRIVATE jrpg_logica)
add_test(NAME pruebas COMMAND pruebas)

# Avisos del compilador
foreach(objetivo jrpg_logica jrpg pruebas)
  if(MSVC)
    target_compile_options(${objetivo} PRIVATE /W4 /utf-8)
  else()
    target_compile_options(${objetivo} PRIVATE -Wall -Wextra)
  endif()
endforeach()

# Copiar assets/ junto al ejecutable
add_custom_command(TARGET jrpg POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:jrpg>/assets)
