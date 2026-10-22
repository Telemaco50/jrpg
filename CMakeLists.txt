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

# --- Lógica del juego (SIN SDL) ---
# Todo lo que no dibuja va en esta librería. La usan el juego Y las pruebas.
# Cada .cpp nuevo de lógica se añade AQUÍ (los .h no hace falta).
add_library(jrpg_logica STATIC
  src/logger.cpp
  src/tipo.cpp
  src/habilidad.cpp
  src/personaje.cpp
  src/combate.cpp
  src/seleccionPersonaje.cpp
  src/controladorPJ.cpp
  src/controladorIA.cpp
)
target_include_directories(jrpg_logica PUBLIC src)

# --- El juego: main + la parte visual (lo que usa SDL) ---
add_executable(jrpg
  src/main.cpp
  src/interfaz.cpp
)
target_link_libraries(jrpg PRIVATE jrpg_logica SDL3_image::SDL3_image SDL3::SDL3)

# --- Pruebas: ejecutable aparte con doctest (no abre ventana ni usa SDL) ---
# doctest es una librería de tests de un solo .h; solo descargamos el código
# (SOURCE_SUBDIR inexistente = no ejecutar su CMakeLists, solo usar la cabecera)
FetchContent_Declare(doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG        v2.4.12
  GIT_SHALLOW    TRUE
  SOURCE_SUBDIR  no-cmake)
FetchContent_MakeAvailable(doctest)

enable_testing()
# Cada archivo de pruebas nuevo (tests/test_*.cpp) se añade a esta lista
add_executable(pruebas
  tests/main_pruebas.cpp
  tests/test_tipo.cpp
)
target_include_directories(pruebas PRIVATE ${doctest_SOURCE_DIR})
target_link_libraries(pruebas PRIVATE jrpg_logica)
add_test(NAME pruebas COMMAND pruebas)

# Avisos del compilador al nivel alto: te ayudan a encontrar bugs pronto
foreach(objetivo jrpg_logica jrpg pruebas)
  if(MSVC)
    target_compile_options(${objetivo} PRIVATE /W4 /utf-8)
  else()
    target_compile_options(${objetivo} PRIVATE -Wall -Wextra)
  endif()
endforeach()

# Copiar la carpeta assets/ junto al .exe después de cada compilación
add_custom_command(TARGET jrpg POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:jrpg>/assets)
