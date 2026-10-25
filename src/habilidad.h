#pragma once
// #include "tipo.h"
#include <string>




enum class Efecto{
    Cura,
    Dano,
    Stun,
    Canalizar   
};



enum class Escalado{
    noEscala,
    turnoLibre,
    turnoCorrea
};


class Habilidad
{

public:
    Habilidad(const std::string& nombre,Efecto efecto,Escalado escalado,int cantidadMin,int cantidadMax,int PM);


    // Getter
    const std::string& getNombre() const  {
      return m_nombre;
    }

   
    // Getter
    Efecto getEfecto() const {
      return m_efecto;
    }


  
    // Getter
    Escalado getEscalado() const {
      return m_escalado;
    }
  
    // Getter
    int getCantidadMin() const {
      return m_cantidadMin;
    }
    // Getter
    int getCantidadMax() const {
      return m_cantidadMax;
    }

 

    // Getter
    int getPM() const {
      return m_PM;
    }


private:
std::string m_nombre;
Efecto m_efecto;
Escalado m_escalado;
int m_cantidadMin;
int m_cantidadMax;
int m_PM;


};