# JRPG

Pequeño juego de combates por turnos al estilo Pokémon, escrito en C++20 con SDL3.
Eliges a uno de los cuatro personajes y te enfrentas 1 contra 1 a un rival controlado por la máquina.

Es un **proyecto personal de aprendizaje**, el segundo después de mi
[emulador de CHIP-8](https://github.com/Telemaco50/chip8). Esta vez no he seguido ningún tutorial:
el diseño del juego y de las clases es propio. Lo he usado para practicar clases y herencia,
polimorfismo, `std::optional`, RAII, generación de números aleatorios y pruebas unitarias.

Los personajes están inspirados en mis amigos, con su permiso.

## Cómo lo he hecho

- **Diseño propio:** tipos, habilidades y reglas del combate pensados por mí.
- **Apoyo de IA:** he usado una IA como profesor: para resolver dudas de C++, revisar mi código,
  escribir parte de las pruebas y montar el entorno (CMake, SDL3, doctest y GitHub Actions).
- **Pruebas primero:** la lógica del juego está separada de SDL y cubierta con pruebas
  ([doctest](https://github.com/doctest/doctest)), así se puede probar sin abrir ninguna ventana.

## El juego

Cada personaje tiene un tipo y tres habilidades (dañar, curar y aturdir).

| Personaje | Tipo | PS | Particularidad |
|---|---|---|---|
| Alvaro | Otaku | 110 | *Turno de magic infinito*: canaliza un turno y, si nadie le interrumpe, deja al rival aturdido |
| Griso | Correa | 100 | Alterna entre "con correa" y "libre" cada 3 turnos; sus habilidades escalan con el estado |
| Josemi | Criptobro | 85 | Daño alto pero poca vida |
| Josito | Dormilón | 100 | Cantidades muy variables |

Tabla de efectividad (ataca → defiende):

| | Criptobro | Otaku | Correa | Dormilón |
|---|---|---|---|---|
| **Criptobro** | — | x1 | x2 | x0.5 |
| **Otaku** | x1 | — | x0.5 | x2 |
| **Correa** | x0.5 | x2 | — | x1 |
| **Dormilón** | x0.5 | x2 | x1 | — |

## Compilar

Requisitos: Visual Studio Build Tools (MSVC), CMake 3.24+ y Git.

```bash
cmake -S . -B build
cmake --build build --config Debug
```

La primera configuración descarga y compila SDL3, SDL3_image y SDL3_ttf, así que tarda unos minutos.

## Ejecutar

```bash
.\build\Debug\jrpg.exe
```

## Controles

| Tecla | Acción |
|---|---|
| Flechas | Mover la selección |
| `Enter` / `Espacio` | Aceptar |
| `Esc` | Salir |

## Pruebas

```bash
ctest --test-dir build -C Debug
```

## Estructura

| Archivo | Qué hace |
|---|---|
| `tipo`, `habilidad`, `personaje` | Datos del juego y reglas de cada personaje |
| `catalogo` | Los cuatro personajes y sus habilidades |
| `combate` | El bucle de turnos: aturdimiento, canalización, correa y efectividad |
| `controlador`, `controladorIA`, `controladorPJ` | Quién decide cada turno: la máquina o el jugador |
| `seleccionPersonaje` | Lógica de la pantalla de selección |
| `interfaz` | Todo lo que usa SDL: ventana, teclado y dibujo |
| `main` | Une las pantallas en el bucle principal |

La lógica (`jrpg_logica`) no depende de SDL; el juego (`jrpg`) y las pruebas (`pruebas`) la enlazan.

## Estado

- ✅ Selección de personaje y combate completo contra la IA.
- ✅ 81 pruebas unitarias.
- ⏳ La IA elige al azar.
- ⏳ Pendiente: PM (puntos de magia) y equilibrar el aturdimiento.

## Créditos

- Fuente [Press Start 2P](https://fonts.google.com/specimen/Press+Start+2P) de CodeMan38, bajo licencia SIL OFL (`assets/fuentes/OFL.txt`).
- Sprites dibujados por mí.

El repositorio tiene un *quality gate* en GitHub Actions que compila el proyecto, pasa las pruebas
y `cppcheck` en cada push a `desarrollo` y en cada pull request a `calidad` o `master`.
