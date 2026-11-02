#include "controladorIA.h"

ControladorIA::ControladorIA(unsigned int semilla)
    : m_generador(semilla)
{
}

std::optional<int> ControladorIA::elegirHabilidad(const Personaje& propio,
                                                  const Personaje& /*rival*/)
{
    int cantidad = static_cast<int>(propio.getHabilidades().size());

    if(cantidad == 0){
        return std::nullopt;
    }

    std::uniform_int_distribution<int> dado(0, cantidad - 1);

    return dado(m_generador);
}
