#pragma once
#include "personaje.h"
#include <vector>

// Pantalla de selección de personaje: SOLO la lógica (qué tarjeta está marcada).
// No dibuja nada: la Interfaz lee este estado y pinta las tarjetas.
//
// Funciona como un cursor sobre la lista de opciones (el catálogo):
//   - las flechas mueven la marca a izquierda o derecha (dando la vuelta en los extremos)
//   - Enter confirma el personaje marcado
//   - después se elige un rival al azar, distinto del elegido
class SeleccionPersonaje {
public:
    // Recibe la lista de personajes entre los que elegir (normalmente crearPersonajes())
    explicit SeleccionPersonaje(const std::vector<Personaje>& opciones);

    // --- Movimiento de la marca (no hacen nada si ya se ha confirmado) ---
    void moverDerecha();      // en el último, vuelve al primero
    void moverIzquierda();    // en el primero, va al último

    // Confirma el personaje marcado. Después ya no se puede mover la marca.
    void confirmar();

    // --- Consultas para la Interfaz y las pruebas ---
    const std::vector<Personaje>& getOpciones() const {
        return m_opciones;
    }

    int getIndiceMarcado() const {
        return m_indiceMarcado;
    }

    bool haConfirmado() const {
        return m_confirmado;
    }

    // El personaje elegido (el marcado). Tiene sentido después de confirmar().
    const Personaje& getElegido() const {
        return m_opciones[m_indiceMarcado];
    }

    // Índice de un rival al azar, SIEMPRE distinto del marcado.
    // La semilla permite que las pruebas sean repetibles.
    int elegirIndiceRival(unsigned int semilla) const;

private:
    std::vector<Personaje> m_opciones;
    int m_indiceMarcado = 0;      // empieza marcado el primero
    bool m_confirmado = false;
};
