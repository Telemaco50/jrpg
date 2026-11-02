#pragma once
#include "personaje.h"
#include "controlador.h"
#include <optional>
#include <random>
#include <string>

// Los dos bandos del combate
enum class Lado {
    Jugador,
    Rival
};


// actualizar() se llama en cada frame y avanza lo que pueda
class Combate {
public:
    // Los personajes se copian; los controladores solo se referencian
    Combate(const Personaje& jugador, const Personaje& rival,
            Controlador& controladorJugador, Controlador& controladorRival,
            unsigned int semilla);

    // Avanza el combate. Se llama en cada frame.
    void actualizar();

    // --- Consultas ---
    bool haTerminado() const {
        return m_ganador.has_value();
    }

    std::optional<Lado> getGanador() const {   // nullopt mientras no haya terminado
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


    const std::string& getUltimoMensaje() const {
        return m_ultimoMensaje;
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
    std::mt19937 m_generador;
    std::string m_ultimoMensaje;           // descripción de la última acción
};
