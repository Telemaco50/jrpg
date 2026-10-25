#pragma once
#include "personaje.h"
#include "controlador.h"
#include <optional>
#include <random>

// Los dos bandos del combate
enum class Lado {
    Jugador,
    Rival
};


//
// No espera a nadie: la Interfaz llama a actualizar() en cada frame, y en cada llamada
// el combate avanza lo que pueda (o nada, si el jugador aún está eligiendo en el menú).
class Combate {
public:
    // Los personajes se COPIAN: el combate trabaja sobre su propia copia,
    // así los del catálogo no se modifican y cada partida empieza con la vida llena.
    // Los controladores NO se copian: el combate solo los usa (pertenecen a quien crea el combate).
    Combate(const Personaje& jugador, const Personaje& rival,
            Controlador& controladorJugador, Controlador& controladorRival,
            unsigned int semilla);

    // Avanza el combate. Se llama en cada frame.
    void actualizar();

    // --- Consultas para la Interfaz y las pruebas ---
    bool haTerminado() const {
        return m_ganador.has_value();
    }

    std::optional<Lado> getGanador() const {   // std::nullopt mientras no haya terminado
        return m_ganador;
    }

    Lado getTurno() const {
        return m_turno;
    }

    const Personaje& getJugador() const {
        return m_jugador;
    }

    const Personaje& getRival() const {
        return m_rival;
    }

private:
    // Aplica la habilidad de 'actor' sobre 'objetivo' (cantidad, escalado, efecto)
    void aplicarHabilidad(Personaje& actor, Personaje& objetivo, const Habilidad& habilidad);

    // Pasa el turno al otro bando
    void cambiarTurno();

    Personaje m_jugador;
    Personaje m_rival;
    Controlador& m_controladorJugador;
    Controlador& m_controladorRival;

    Lado m_turno = Lado::Jugador;          // empieza el jugador
    std::optional<Lado> m_ganador;         // vacío mientras se juega
    std::mt19937 m_generador;              // para las cantidades aleatorias (mín..máx)
};
