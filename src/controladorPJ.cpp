#include "controladorPJ.h"

// La Interfaz llama a esto cuando el jugador pulsa una opción del menú
void ControladorPJ::recibirEleccion(int indice){
    m_pendiente=indice;
}

std::optional<int> ControladorPJ::elegirHabilidad(const Personaje& propio,
                                                  const Personaje& /*rival*/)
{
    // 1. ¿Ha elegido algo? Si no, "todavía no"
    if(!m_pendiente.has_value()){
        return std::nullopt;
    }

    // 2. Copiar la elección y VACIAR el miembro (primero copiar, luego vaciar):
    //    así no se repite sola en el siguiente turno
    int eleccion=m_pendiente.value();
    m_pendiente.reset();

    // 3. Comprobar que esa habilidad existe (índices válidos: 0 .. cantidad-1)
    int cantidad=static_cast<int>(propio.getHabilidades().size());
    if(eleccion<0 || eleccion>=cantidad){
        return std::nullopt;
    }

    return eleccion;
}
