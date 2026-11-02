// Pruebas de la tabla de efectividad
#include <doctest/doctest.h>
#include "tipo.h"

TEST_CASE("Fila Criptobro: Criptobro atacando")
{
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Otaku)       == doctest::Approx(1.0f));  // normal
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Correa)      == doctest::Approx(2.0f));  // muy eficaz
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Dormilon) == doctest::Approx(0.5f));  // poco eficaz
}

TEST_CASE("Fila Otaku: Otaku atacando")
{
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Criptobro)   == doctest::Approx(1.0f));  // normal
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Correa)      == doctest::Approx(0.5f));  // poco eficaz
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Dormilon) == doctest::Approx(2.0f));  // muy eficaz
}

TEST_CASE("Fila Correa: Correa atacando")
{
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Criptobro)   == doctest::Approx(0.5f));  // poco eficaz
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Otaku)       == doctest::Approx(2.0f));  // muy eficaz
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Dormilon) == doctest::Approx(1.0f));  // normal
}

TEST_CASE("Fila Dormilon: Dormilon atacando")
{
    CHECK(efectividadAtaque(Tipo::Dormilon, Tipo::Criptobro) == doctest::Approx(0.5f));  // poco eficaz
    CHECK(efectividadAtaque(Tipo::Dormilon, Tipo::Otaku)     == doctest::Approx(2.0f));  // muy eficaz
    CHECK(efectividadAtaque(Tipo::Dormilon, Tipo::Correa)    == doctest::Approx(1.0f));  // normal
}
