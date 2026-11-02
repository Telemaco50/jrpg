#pragma once
#include "personaje.h"
#include <vector>

// Lógica de la pantalla de selección (la Interfaz la dibuja)
class SeleccionPersonaje {
public:
    explicit SeleccionPersonaje(const std::vector<Personaje>& opciones);

    // No hacen nada si ya se ha confirmado
    void moverDerecha();      // en el último, vuelve al primero
    void moverIzquierda();    // en el primero, va al último

    void confirmar();

    // --- Consultas ---
    const std::vector<Personaje>& getOpciones() const {
        return m_opciones;
    }

    int getIndiceMarcado() const {
        return m_indiceMarcado;
    }

    bool haConfirmado() const {
        return m_confirmado;
    }

    // Tiene sentido después de confirmar()
    const Personaje& getElegido() const {
        return m_opciones[m_indiceMarcado];
    }

    // Índice de un rival al azar, distinto del marcado
    int elegirIndiceRival(unsigned int semilla) const;

private:
    std::vector<Personaje> m_opciones;
    int m_indiceMarcado = 0;
    bool m_confirmado = false;
};
