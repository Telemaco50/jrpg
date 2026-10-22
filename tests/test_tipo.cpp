// Pruebas de tipo.h / tipo.cpp
//
// Cada TEST_CASE es una prueba con nombre. Dentro, cada CHECK es una comprobación:
// si lo que hay dentro de CHECK(...) es falso, doctest te dice qué línea ha fallado
// y con qué valores, y sigue con las demás.
//
// Es como un test de calidad de datos: se comprueba cada casilla de la tabla de
// efectividad de docs/DISENO.md (ataca = fila, defiende = columna).
// La diagonal (mismo tipo contra sí mismo) no se prueba: nunca se da en partida.

#include <doctest/doctest.h>
#include "tipo.h"

TEST_CASE("Fila Criptobro: Criptobro atacando")
{
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Otaku)       == doctest::Approx(1.0f));  // normal
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Correa)      == doctest::Approx(2.0f));  // muy eficaz
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Narcolepsia) == doctest::Approx(0.5f));  // poco eficaz
}

TEST_CASE("Fila Otaku: Otaku atacando")
{
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Criptobro)   == doctest::Approx(1.0f));  // normal
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Correa)      == doctest::Approx(0.5f));  // poco eficaz
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Narcolepsia) == doctest::Approx(2.0f));  // muy eficaz
}

TEST_CASE("Fila Correa: Correa atacando")
{
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Criptobro)   == doctest::Approx(0.5f));  // poco eficaz
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Otaku)       == doctest::Approx(2.0f));  // muy eficaz
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Narcolepsia) == doctest::Approx(1.0f));  // normal
}

TEST_CASE("Fila Narcolepsia: Narcolepsia atacando")
{
    CHECK(efectividadAtaque(Tipo::Narcolepsia, Tipo::Criptobro) == doctest::Approx(0.5f));  // poco eficaz
    CHECK(efectividadAtaque(Tipo::Narcolepsia, Tipo::Otaku)     == doctest::Approx(2.0f));  // muy eficaz
    CHECK(efectividadAtaque(Tipo::Narcolepsia, Tipo::Correa)    == doctest::Approx(1.0f));  // normal
}
