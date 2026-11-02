#pragma once
#include "tipo.h"
#include "habilidad.h"
#include <string>
#include <vector>

// Personaje: datos fijos y estado del combate.
// El estado solo cambia a través de sus métodos (sin setters).
class Personaje {
public:
    Personaje(const std::string& nombre, Tipo tipo, int psMax,
              const std::vector<Habilidad>& habilidades, const std::string& nombreSprite);

    // --- Getters de los datos fijos ---
    const std::string& getNombre() const {
        return m_nombre;
    }

    Tipo getTipo() const {
        return m_tipo;
    }

    int getPsMax() const {
        return m_psMax;
    }

    const std::vector<Habilidad>& getHabilidades() const {
        return m_habilidades;
    }

    const std::string& getNombreSprite() const {
        return m_nombreSprite;
    }


    // --- Getters del estado ---
    int getPsActuales() const {
        return m_psActuales;
    }

    int getTurnosAturdido() const {
        return m_turnosAturdido;
    }

    int getPM() const {
        return m_PM;
    }


    // --- Consultas ---
    bool estaVivo() const;
    bool estaAturdido() const;

    // --- Acciones ---
    void recibirDanio(int cantidad);  // no baja de 0
    void curar(int cantidad);         // no supera el máximo
    void aturdir(int turnos);
    void pasarTurno();                // resta aturdimiento y avanza la correa


    bool estaCanalizando() const;
    void empezarCanalizacion(int turnosAturdimiento);
    int completarCanalizacion();                        // devuelve los turnos de aturdimiento
    // aturdir() interrumpe la canalización

    // --- Correa (tipo Correa) ---
    bool tieneCorrea() const {
        return m_conCorrea;
    }

    int turnosConCorrea() const {  // 0 si está libre
        return m_turnosConCorrea;
    }

    int turnosLibre() const {  // 0 si lleva la correa
        return m_turnosLibre;
    }

    void ponerCorrea();               // contadores a 0
    void quitarCorrea();              // contadores a 0

private:
    // Datos fijos
    std::string m_nombre;
    Tipo m_tipo;
    int m_psMax;
    std::vector<Habilidad> m_habilidades;
    std::string m_nombreSprite;

    // Estado del combate
    int m_psActuales = 0;
    int m_turnosAturdido = 0;
    int m_canalizacion = 0;        // 0 = no canaliza
    int m_PM = 0;                  // pendiente de diseñar

    // Correa
    bool m_conCorrea = false;
    int m_turnosConCorrea = 0;
    int m_turnosLibre = 0;
};
