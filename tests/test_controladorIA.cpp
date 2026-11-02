// Pruebas de ControladorIA (semilla fija)
#include <doctest/doctest.h>
#include "controladorIA.h"

// Personaje con 3 habilidades
static Personaje crearConHabilidades()
{
    return Personaje("Prueba", Tipo::Otaku, 100, {
        Habilidad("Uno",  Efecto::Dano, Escalado::noEscala, 10, 10, 0),
        Habilidad("Dos",  Efecto::Cura, Escalado::noEscala, 10, 10, 0),
        Habilidad("Tres", Efecto::Stun, Escalado::noEscala, 1, 1, 0),
    }, "prueba");
}

static Personaje crearSinHabilidades()
{
    return Personaje("Vacio", Tipo::Otaku, 100, {}, "vacio");
}

TEST_CASE("La IA siempre decide (nunca responde 'todavía no')")
{
    ControladorIA ia(12345);
    Personaje propio = crearConHabilidades();
    Personaje rival = crearConHabilidades();

    CHECK(ia.elegirHabilidad(propio, rival).has_value());
}

TEST_CASE("La IA siempre devuelve un índice válido")
{
    ControladorIA ia(12345);
    Personaje propio = crearConHabilidades();
    Personaje rival = crearConHabilidades();

    for(int i = 0; i < 100; i++){
        std::optional<int> eleccion = ia.elegirHabilidad(propio, rival);
        REQUIRE(eleccion.has_value());
        CHECK(eleccion.value() >= 0);
        CHECK(eleccion.value() <= 2);
    }
}

TEST_CASE("La IA usa todas sus habilidades (no elige siempre la misma)")
{
    ControladorIA ia(12345);
    Personaje propio = crearConHabilidades();
    Personaje rival = crearConHabilidades();

    int vecesElegida[3] = {0, 0, 0};
    for(int i = 0; i < 100; i++){
        std::optional<int> eleccion = ia.elegirHabilidad(propio, rival);
        REQUIRE(eleccion.has_value());
        vecesElegida[eleccion.value()]++;
    }

    CHECK(vecesElegida[0] > 0);
    CHECK(vecesElegida[1] > 0);
    CHECK(vecesElegida[2] > 0);
}

TEST_CASE("Sin habilidades, la IA no puede elegir nada")
{
    ControladorIA ia(12345);
    Personaje propio = crearSinHabilidades();
    Personaje rival = crearConHabilidades();

    CHECK_FALSE(ia.elegirHabilidad(propio, rival).has_value());
}

TEST_CASE("La misma semilla da las mismas decisiones")
{
    ControladorIA ia1(777);
    ControladorIA ia2(777);
    Personaje propio = crearConHabilidades();
    Personaje rival = crearConHabilidades();

    for(int i = 0; i < 20; i++){
        CHECK(ia1.elegirHabilidad(propio, rival) == ia2.elegirHabilidad(propio, rival));
    }
}
