#pragma once
#include "controlador.h"
#include <random>

// Controlador de la máquina: decide sola qué habilidad usar.
// Primera versión: elige una de sus habilidades al azar.
class ControladorIA : public Controlador {
public:
    // La semilla fija la secuencia de números aleatorios:
    // en las pruebas se pasa una fija (resultado predecible),
    // en el juego una distinta cada partida (std::random_device).
    explicit ControladorIA(unsigned int semilla);

    std::optional<int> elegirHabilidad(const Personaje& propio,
                                       const Personaje& rival) override;

private:
    // Miembro (no variable local) para que conserve su estado entre turnos
    std::mt19937 m_generador;
};
