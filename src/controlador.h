#pragma once
#include <optional>
#include "personaje.h"

// "Cerebro" que decide qué habilidad usa un personaje en su turno.
// Es una clase ABSTRACTA: solo define la pregunta. La responden las clases hijas:
//   - ControladorIA: decide sola, al momento.
//   - ControladorPJ: responde lo que elija el jugador en el menú.
class Controlador {
public:
    virtual ~Controlador() = default;

    // Pregunta del turno: "¿qué habilidad usas?"
    // Devuelve el índice de la habilidad (posición en propio.getHabilidades()),
    // o std::nullopt si todavía no ha decidido.
    virtual std::optional<int> elegirHabilidad(const Personaje& propio,
                                               const Personaje& rival) = 0;
};