#include "logger.h"
#include "personaje.h"
constexpr int kTurnosPorEstado = 3;

Personaje::Personaje(const std::string& nombre, Tipo tipo, 
    int psMax, const std::vector<Habilidad>& habilidades, const std::string& nombreSprite)
{
    m_nombre=nombre;
    m_tipo=tipo;
    m_psMax=psMax;
    m_habilidades=habilidades;
    m_nombreSprite=nombreSprite;
    m_psActuales=m_psMax;
    m_turnosAturdido=0;

    if(m_tipo==Tipo::Correa){
        m_conCorrea=true;
    }

}


void Personaje::recibirDanio(int danio){
    if((m_psActuales-danio)<0){
        m_psActuales=0;
    }else{
        m_psActuales=m_psActuales-danio;
    }

}

void Personaje::curar(int cantidad){
    if((m_psActuales+cantidad)>m_psMax){
        m_psActuales=m_psMax;
    }else{
        m_psActuales=m_psActuales+cantidad;
    }

}

bool Personaje::estaVivo()const{
    return m_psActuales>0;
}

bool Personaje::estaAturdido()const{
    return m_turnosAturdido>0;
}

void Personaje::aturdir(int turno){
    m_turnosAturdido=m_turnosAturdido+turno;

    // Aturdir a alguien que está canalizando le rompe la canalización
    if(turno>0){
        m_canalizacion=0;
    }

}

// Al cambiar de estado, los contadores vuelven a 0
void Personaje::ponerCorrea(){
    m_conCorrea=true;
    m_turnosConCorrea=0;
    m_turnosLibre=0;
}

void Personaje::quitarCorrea(){
    m_conCorrea=false;
    m_turnosConCorrea=0;
    m_turnosLibre=0;
}

void Personaje::pasarTurno(){
    // Aturdimiento
    if(m_turnosAturdido>0){
        m_turnosAturdido--;
    }

    // Correa
    if(m_tipo==Tipo::Correa){
        if(m_conCorrea){
            m_turnosConCorrea++;
            if(m_turnosConCorrea>=kTurnosPorEstado){
                quitarCorrea();
            }
        }else{
            m_turnosLibre++;
            if(m_turnosLibre>=kTurnosPorEstado){
                ponerCorrea();
            }
        }
    }
}

// --- Canalización ---
bool Personaje::estaCanalizando()const{
    return m_canalizacion>0;
}

void Personaje::empezarCanalizacion(int turnosAturdimiento){
    m_canalizacion=turnosAturdimiento;
}

int Personaje::completarCanalizacion(){

    int turnos=m_canalizacion;
    m_canalizacion=0;
    return turnos;
}
