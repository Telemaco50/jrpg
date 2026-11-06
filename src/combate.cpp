#include "combate.h"
#include "tipo.h"
#include <algorithm>

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
// Bando contrario a 'lado'
static Lado ladoContrario(Lado lado){
    return (lado == Lado::Jugador) ? Lado::Rival : Lado::Jugador;
}

void Combate::actualizar(){
    if(haTerminado()){
        return;
    }

    bool turnoDelJugador = (m_turno==Lado::Jugador);
    Personaje& actor = turnoDelJugador ? m_jugador : m_rival;
    Personaje& otro  = turnoDelJugador ? m_rival   : m_jugador;
    Controlador& controlador = turnoDelJugador ? m_controladorJugador : m_controladorRival;

    // Aturdido: pierde el turno
    if(actor.estaAturdido()){
        m_ultimoMensaje = actor.getNombre() + " está aturdido y pierde el turno.";
        actor.pasarTurno();
        cambiarTurno();
        return;
    }

    // Canalización sin interrumpir: se completa y gasta el turno
    if(actor.estaCanalizando()){
        int turnos = actor.completarCanalizacion();
        otro.aturdir(turnos);
        m_ultimoMensaje = "¡" + actor.getNombre() + " completa el hechizo! " + otro.getNombre() +
                          " queda aturdido " + std::to_string(turnos) + " turnos.";

        Evento evento;
        evento.numero = m_ultimoEvento.numero + 1;
        evento.efecto = Efecto::Stun;
        evento.objetivo = ladoContrario(m_turno);
        evento.cantidad = turnos;
        m_ultimoEvento = evento;

        actor.pasarTurno();
        cambiarTurno();
        return;
    }

    // Sin decisión todavía: se vuelve a preguntar en el siguiente frame
    std::optional<int> decision = controlador.elegirHabilidad(actor, otro);
    if(!decision.has_value()){
        return;
    }

    const Habilidad& habilidad = actor.getHabilidades()[decision.value()];
    aplicarHabilidad(actor, otro, habilidad);

    // Si cae el objetivo, gana el actor
    if(!otro.estaVivo()){
        m_ganador = m_turno;
        return;
    }

    // Fin del turno
    bool teniaCorrea = actor.tieneCorrea();
    actor.pasarTurno();
    if(actor.getTipo() == Tipo::Correa && actor.tieneCorrea() != teniaCorrea){
        m_ultimoMensaje += actor.tieneCorrea() ? " ¡A " + actor.getNombre() + " le ponen la correa!"
                                               : " ¡" + actor.getNombre() + " se quita la correa!";
    }
    cambiarTurno();
}

void Combate::aplicarHabilidad(Personaje& actor, Personaje& objetivo, const Habilidad& habilidad){
    // Cantidad base entre mínimo y máximo
    std::uniform_int_distribution<int> dado(habilidad.getCantidadMin(), habilidad.getCantidadMax());
    int cantidad = dado(m_generador);

    // Escalado de la correa: max(1, contador)
    int multiplicador = 1;
    if(habilidad.getEscalado()==Escalado::turnoLibre){
        multiplicador = std::max(1, actor.turnosLibre());
    }
    else if(habilidad.getEscalado()==Escalado::turnoCorrea){
        multiplicador = std::max(1, actor.turnosConCorrea());
    }
    cantidad = cantidad * multiplicador;

    m_ultimoMensaje = actor.getNombre() + " usa " + habilidad.getNombre() + ".";

    // Evento nuevo en cada acción: empieza con los valores por defecto
    Evento evento;
    evento.numero = m_ultimoEvento.numero + 1;
    evento.efecto = habilidad.getEfecto();
    evento.objetivo = ladoContrario(m_turno);

    switch(habilidad.getEfecto()){
        case Efecto::Dano:
        {
            // Efectividad del tipo (redondeo hacia abajo)
            float efectividad = efectividadAtaque(actor.getTipo(), objetivo.getTipo());
            int psAntes = objetivo.getPsActuales();
            objetivo.recibirDanio(static_cast<int>(cantidad * efectividad));
            int danio = psAntes - objetivo.getPsActuales();

            evento.cantidad = danio;
            evento.efectividad = efectividad;

            if(efectividad > 1.0f){
                m_ultimoMensaje += " ¡Cómo pega esto!";
            }else if(efectividad < 1.0f){
                m_ultimoMensaje += " No hace ni un rasguño...";
            }
            m_ultimoMensaje += " -" + std::to_string(danio) + " PS";
            break;
        }
        case Efecto::Cura:
        {
            int psAntes = actor.getPsActuales();
            actor.curar(cantidad);
            int curado = actor.getPsActuales() - psAntes;

            evento.objetivo = m_turno;      // la cura es para uno mismo
            evento.cantidad = curado;

            m_ultimoMensaje += " +" + std::to_string(curado) + " PS";
            break;
        }
        case Efecto::Stun:
            objetivo.aturdir(cantidad);
            evento.cantidad = cantidad;

            m_ultimoMensaje += " " + objetivo.getNombre() + " queda aturdido " +
                               std::to_string(cantidad) + (cantidad == 1 ? " turno." : " turnos.");
            break;
        case Efecto::Canalizar:
            actor.empezarCanalizacion(cantidad);
            evento.objetivo = m_turno;      // el efecto visual va sobre quien canaliza
            evento.cantidad = cantidad;

            m_ultimoMensaje += " Empieza a canalizar...";
            break;
    }

    m_ultimoEvento = evento;
}