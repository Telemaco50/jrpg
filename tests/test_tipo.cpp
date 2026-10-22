// Pruebas de tipo.h / tipo.cpp
//
// Cada TEST_CASE es una prueba con nombre. Dentro, cada CHECK es una comprobación:
// si lo que hay dentro de CHECK(...) es falso, doctest te dice qué línea ha fallado
// y con qué valores, y sigue con las demás.
//
// Es como un test de calidad de datos:
//   "SELECT multiplicador FROM efectividad WHERE atacante='Otaku' AND defensor='Narcolepsia'"
//   → debe dar 2.

#include <doctest/doctest.h>
#include "tipo.h"

TEST_CASE("Otaku es eficaz contra Narcolepsia")
{
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Narcolepsia) == doctest::Approx(2.0f));
}

TEST_CASE("Criptobro es poco eficaz contra Narcolepsia")
{
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Narcolepsia) == doctest::Approx(0.5f));
}

TEST_CASE("Criptobro contra Otaku es normal")
{
    CHECK(efectividadAtaque(Tipo::Criptobro, Tipo::Otaku) == doctest::Approx(1.0f));
}

// TODO (tu turno): añade las comprobaciones que faltan de la tabla de docs/DISENO.md.
// Pista: puedes poner varios CHECK dentro del mismo TEST_CASE, por ejemplo uno por fila:
//
TEST_CASE("Correa es poco eficaz contra Cripto bro")
{
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Criptobro) == doctest::Approx(0.5f));
   
}

TEST_CASE("Correa es muy eficaz contra Otaku")
{
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Otaku) == doctest::Approx(2.0f));
   
}

TEST_CASE("Correa es  eficaz contra Narcolepsia")
{
    CHECK(efectividadAtaque(Tipo::Correa, Tipo::Narcolepsia) == doctest::Approx(1.0f));
   
}

//Test Otaku

TEST_CASE("Otaku es  eficaz contra Cripto bro")
{
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Criptobro) == doctest::Approx(1.0f));
   
}

TEST_CASE("Correa es poco eficaz contra Otaku")
{
    CHECK(efectividadAtaque(Tipo::Otaku, Tipo::Correa) == doctest::Approx(0.5f));
   
}



