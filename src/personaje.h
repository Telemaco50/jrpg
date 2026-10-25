#pragma once
#include "tipo.h"
#include "habilidad.h"
#include <string>
#include <vector>

// Un personaje del juego (Alvaro, Griso, Josemi, Josito).
//
// Tiene dos clases de datos:
//   - Datos fijos: se rellenan en el constructor y no cambian nunca (solo getters).
//   - Estado del combate: cambia durante la pelea, pero SOLO a través de los
//     métodos con reglas (recibirDanio, curar, aturdir, pasarTurno...), nunca con setters.
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


    // --- Preguntas sobre el estado (const: solo consultan) ---
    bool estaVivo() const;          // ¿PS > 0?
    bool estaAturdido() const;      // ¿le quedan turnos aturdido?

    // --- Acciones con reglas (sin const: cambian el estado) ---
    void recibirDanio(int cantidad);  // resta PS, pero nunca por debajo de 0
    void curar(int cantidad);         // suma PS, pero nunca por encima del máximo
    void aturdir(int turnos);         // suma turnos de aturdimiento
    void pasarTurno();                // al acabar su turno: resta aturdimiento y avanza los contadores de correa


    bool estaCanalizando() const;                       // ¿está canalizando?
    void empezarCanalizacion(int turnosAturdimiento);   // empieza; guarda los turnos que lanzará al completarla
    int completarCanalizacion();                        // termina la canalización y devuelve esos turnos
    // Nota: aturdir() a un personaje que está canalizando le ROMPE la canalización

    // --- Correa (solo la usa el tipo Correa: Griso) ---
    bool tieneCorrea() const {
        return m_conCorrea;
    }

    int turnosConCorrea() const {  // 0 si está libre
        return m_turnosConCorrea;
    }

    int turnosLibre() const {  // 0 si lleva la correa
        return m_turnosLibre;
    }

    void ponerCorrea();               // cambia de estado: contadores a 0 (turno de cambio)
    void quitarCorrea();              // cambia de estado: contadores a 0 (turno de cambio)

private:
    // Datos fijos
    std::string m_nombre;
    Tipo m_tipo;
    int m_psMax;
    std::vector<Habilidad> m_habilidades;
    std::string m_nombreSprite;

    // Estado del combate (con valor inicial para que nunca empiecen con basura)
    int m_psActuales = 0;          // el constructor lo pone a m_psMax (vida llena)
    int m_turnosAturdido = 0;
    int m_canalizacion = 0;        // 0 = no canaliza; > 0 = turnos de aturdimiento que lanzará al completarla
    int m_PM = 0;                  // PM: pendiente de diseñar

    // Estado de la correa (Griso). TODO: decidir si empieza con correa o libre
    bool m_conCorrea = false;
    int m_turnosConCorrea = 0;
    int m_turnosLibre = 0;
};
