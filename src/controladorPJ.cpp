#include "controladorPJ.h"

void ControladorPJ::recibirEleccion(int indice){
    m_pendiente=indice;
}

std::optional<int> ControladorPJ::elegirHabilidad(const Personaje& propio,
                                                  const Personaje& /*rival*/)
{
    if(!m_pendiente.has_value()){
        return std::nullopt;
    }

    // La elección se entrega una sola vez
    int eleccion=m_pendiente.value();
    m_pendiente.reset();

    // Índice fuera de rango: se ignora
    int cantidad=static_cast<int>(propio.getHabilidades().size());
    if(eleccion<0 || eleccion>=cantidad){
        return std::nullopt;
    }

    return eleccion;
}
