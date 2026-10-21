# JRPG

JRPG tipo Pokémon en C++20 con SDL3, hecho para aprender C++.

## Compilar

Requisitos: Visual Studio Build Tools (MSVC), CMake 3.24+, Git.

```bash
cmake -S . -B build
cmake --build build --config Debug
```

La primera configuración descarga y compila SDL3 y SDL3_image (tarda varios minutos).

En VS Code: **F7** compila y **Shift+F5** ejecuta.

## Ramas

- `desarrollo`: trabajo diario
- `calidad`: lo que ha pasado el quality gate (compilación + cppcheck)
- `master`: versión estable
