// Pruebas de habilidad.h / habilidad.cpp
//
// Se usan valores DISTINTOS en cada dato a propósito (por ejemplo mínimo 20 y máximo 30):
// si el constructor guardara un dato en el miembro equivocado, alguna prueba fallaría.

#include <doctest/doctest.h>
#include "habilidad.h"

TEST_CASE("El constructor guarda todos los datos en su sitio")
{
    Habilidad ramen("Ingesta de ramen", Efecto::Cura, Escalado::noEscala, 20, 30, 5);

    CHECK(ramen.getNombre() == "Ingesta de ramen");
    CHECK(ramen.getEfecto() == Efecto::Cura);
    CHECK(ramen.getEscalado() == Escalado::noEscala);
    CHECK(ramen.getCantidadMin() == 20);
    CHECK(ramen.getCantidadMax() == 30);
    CHECK(ramen.getPM() == 5);
}

TEST_CASE("Habilidad de cantidad fija: mínimo y máximo iguales")
{
    Habilidad hijoUnico("Hijo único", Efecto::Dano, Escalado::noEscala, 15, 15, 0);

    CHECK(hijoUnico.getEfecto() == Efecto::Dano);
    CHECK(hijoUnico.getCantidadMin() == hijoUnico.getCantidadMax());
}

TEST_CASE("Habilidad con escalado (Griso)")
{
    Habilidad uni("La puta uni", Efecto::Dano, Escalado::turnoCorrea, 5, 5, 0);
    Habilidad sermon("Sermón monumental", Efecto::Stun, Escalado::turnoLibre, 1, 1, 0);

    CHECK(uni.getEscalado() == Escalado::turnoCorrea);
    CHECK(sermon.getEfecto() == Efecto::Stun);
    CHECK(sermon.getEscalado() == Escalado::turnoLibre);
}
