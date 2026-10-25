#pragma once
#include "controlador.h"


class ControladorPJ : public Controlador {
public:

void recibirEleccion(int indice);

std::optional<int> elegirHabilidad(const Personaje& propio,
                                       const Personaje& rival) override;

private:
std::optional<int> m_pendiente;
};