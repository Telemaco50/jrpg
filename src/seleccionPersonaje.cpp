#include "logger.h"
#include "seleccionPersonaje.h"
#include <random>

SeleccionPersonaje::SeleccionPersonaje(const std::vector<Personaje>& opciones){
m_opciones=opciones;

};

void SeleccionPersonaje::moverDerecha(){
    // Ya confirmado: no se mueve
    if(m_confirmado){
        return;
    }

    int cantidad = static_cast<int>(m_opciones.size());

    // Del último vuelve al primero
    if(m_indiceMarcado == cantidad - 1){
        m_indiceMarcado = 0;
    }else{
        m_indiceMarcado++;
    }
}


void SeleccionPersonaje::moverIzquierda(){
    // Ya confirmado: no se mueve
    if(m_confirmado){
        return;
    }

    int cantidad = static_cast<int>(m_opciones.size());

    // Del primero va al último
    if(m_indiceMarcado == 0){
        m_indiceMarcado = cantidad - 1;
    }else{
        m_indiceMarcado--;
    }
}

void SeleccionPersonaje::confirmar(){
    m_confirmado= true;

};


int SeleccionPersonaje::elegirIndiceRival(unsigned int semilla) const{
    std::mt19937 generador(semilla);
    int cantidad = static_cast<int>(m_opciones.size());

    std::uniform_int_distribution<int> dado(0, cantidad - 2);
    int rival = dado(generador);

    if(rival >= m_indiceMarcado){
        rival++;
    }
    return rival;
}
