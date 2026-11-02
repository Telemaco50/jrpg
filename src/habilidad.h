#pragma once
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


    const std::string& getNombre() const  {
      return m_nombre;
    }

   
    Efecto getEfecto() const {
      return m_efecto;
    }


  
    Escalado getEscalado() const {
      return m_escalado;
    }
  
    int getCantidadMin() const {
      return m_cantidadMin;
    }
    int getCantidadMax() const {
      return m_cantidadMax;
    }

 

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