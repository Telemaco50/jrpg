#include "logger.h"
#include "habilidad.h"

Habilidad::Habilidad(const std::string& nombre, Efecto efecto, Escalado escalado, int cantidadMin, int cantidadMax, int PM)
{
    m_nombre=nombre;
    m_efecto=efecto;
    m_escalado=escalado;
    m_cantidadMin=cantidadMin;
    m_cantidadMax=cantidadMax;
    m_PM=PM;
}
