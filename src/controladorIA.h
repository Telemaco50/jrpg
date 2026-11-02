#pragma once
#include "controlador.h"
#include <random>

// Controlador de la máquina: elige una habilidad al azar
class ControladorIA : public Controlador {
public:
    // Semilla fija en las pruebas; std::random_device en el juego
    explicit ControladorIA(unsigned int semilla);

    std::optional<int> elegirHabilidad(const Personaje& propio,
                                       const Personaje& rival) override;

private:
    std::mt19937 m_generador;
};
