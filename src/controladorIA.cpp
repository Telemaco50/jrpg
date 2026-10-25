#include "controladorIA.h"

ControladorIA::ControladorIA(unsigned int semilla)
    : m_generador(semilla)
{
}

std::optional<int> ControladorIA::elegirHabilidad(const Personaje& propio,
                                                  const Personaje& /*rival*/)
{
    // ¿Cuántas habilidades tiene? (size() devuelve size_t; lo pasamos a int)
    int cantidad = static_cast<int>(propio.getHabilidades().size());

    // Sin habilidades no hay nada que elegir
    if(cantidad == 0){
        return std::nullopt;
    }

    // Un "dado" con tantas caras como habilidades: índices de 0 a cantidad-1
    std::uniform_int_distribution<int> dado(0, cantidad - 1);

    // Tirarlo con NUESTRO generador (el miembro, que avanza en cada tirada)
    return dado(m_generador);
}
