#include "combate.h"
#include "tipo.h"
#include <algorithm>

// Los personajes y los controladores DEBEN ir en la lista de inicialización:
//  - Personaje no tiene constructor sin parámetros: no puede nacer "vacío" y rellenarse después.
//  - Una referencia (Controlador&) tiene que saber a qué apunta desde que nace.
Combate::Combate(const Personaje& jugador, const Personaje& rival,
                 Controlador& controladorJugador, Controlador& controladorRival,
                 unsigned int semilla)
    : m_jugador(jugador),
      m_rival(rival),
      m_controladorJugador(controladorJugador),
      m_controladorRival(controladorRival),
      m_generador(semilla)
{
}

void Combate::cambiarTurno(){
    if(m_turno==Lado::Jugador){
        m_turno=Lado::Rival;
    }
    else
    {
        m_turno=Lado::Jugador;
    }
}

void Combate::actualizar(){
    // 0. Si ya hay ganador, no hay nada más que hacer
    if(haTerminado()){
        return;
    }

    // 1. ¿Quién actúa en este turno? Se guardan como REFERENCIAS (&) a los de verdad:
    //    sin el &, serían copias y el daño se le haría a la copia, no al personaje del combate.
    //    (el "? :" es un if/else en una línea: condición ? si_es_verdad : si_es_falso)
    bool turnoDelJugador = (m_turno==Lado::Jugador);
    Personaje& actor = turnoDelJugador ? m_jugador : m_rival;
    Personaje& otro  = turnoDelJugador ? m_rival   : m_jugador;
    Controlador& controlador = turnoDelJugador ? m_controladorJugador : m_controladorRival;

    // 2. Si está aturdido, pierde el turno (sin preguntar nada a su controlador)
    if(actor.estaAturdido()){
        actor.pasarTurno();      // se gasta un turno de aturdimiento
        cambiarTurno();
        return;
    }

    // 3. Preguntar al controlador qué hace. Si aún no ha decidido (jugador en el menú), esperar:
    //    en el siguiente frame se volverá a preguntar.
    std::optional<int> decision = controlador.elegirHabilidad(actor, otro);
    if(!decision.has_value()){
        return;
    }

    // 4. Aplicar la habilidad elegida
    const Habilidad& habilidad = actor.getHabilidades()[decision.value()];
    aplicarHabilidad(actor, otro, habilidad);

    // 5. ¿Ha caído el otro? Entonces gana el que acaba de actuar
    if(!otro.estaVivo()){
        m_ganador = m_turno;
        return;
    }

    // 6. Fin del turno: el actor "pasa turno" (aturdimiento, correa) y le toca al otro
    actor.pasarTurno();
    cambiarTurno();
}

void Combate::aplicarHabilidad(Personaje& actor, Personaje& objetivo, const Habilidad& habilidad){
    //  Cantidad base: al azar entre el mínimo y el máximo (si son iguales, siempre la misma)
    std::uniform_int_distribution<int> dado(habilidad.getCantidadMin(), habilidad.getCantidadMax());
    int cantidad = dado(m_generador);

    // Escalado (Griso): multiplicador = max(1, contador del estado con el que escala)
    int multiplicador = 1;
    if(habilidad.getEscalado()==Escalado::turnoLibre){
        multiplicador = std::max(1, actor.turnosLibre());
    }
    else if(habilidad.getEscalado()==Escalado::turnoCorrea){
        multiplicador = std::max(1, actor.turnosConCorrea());
    }
    cantidad = cantidad * multiplicador;

    switch(habilidad.getEfecto()){
        case Efecto::Dano:
        {
            // × efectividad del tipo; static_cast<int> redondea hacia abajo (7.5 → 7)
            float efectividad = efectividadAtaque(actor.getTipo(), objetivo.getTipo());
            objetivo.recibirDanio(static_cast<int>(cantidad * efectividad));
            break;
        }
        case Efecto::Cura:
            actor.curar(cantidad);
            break;
        case Efecto::Stun:
            objetivo.aturdir(cantidad);
            break;
    }
}
