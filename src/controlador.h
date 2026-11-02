#pragma once
#include <optional>
#include "personaje.h"

// Decide qué habilidad usa un personaje en su turno (IA o jugador)
class Controlador {
public:
    virtual ~Controlador() = default;

    // Índice de la habilidad elegida, o nullopt si todavía no ha decidido
    virtual std::optional<int> elegirHabilidad(const Personaje& propio,
                                               const Personaje& rival) = 0;
};