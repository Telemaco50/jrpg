// Pruebas de ControladorPJ
#include <doctest/doctest.h>
#include "controladorPJ.h"

static Personaje crearConTresHabilidades()
{
    return Personaje("Prueba", Tipo::Otaku, 100, {
        Habilidad("Uno",  Efecto::Dano, Escalado::noEscala, 10, 10, 0),
        Habilidad("Dos",  Efecto::Cura, Escalado::noEscala, 10, 10, 0),
        Habilidad("Tres", Efecto::Stun, Escalado::noEscala, 1, 1, 0),
    }, "prueba");
}

TEST_CASE("Jugador: sin elegir nada, responde 'todavía no'")
{
    ControladorPJ pj;
    Personaje propio = crearConTresHabilidades();
    Personaje rival = crearConTresHabilidades();

    CHECK_FALSE(pj.elegirHabilidad(propio, rival).has_value());
}

TEST_CASE("Jugador: devuelve la habilidad que se ha elegido en el menú")
{
    ControladorPJ pj;
    Personaje propio = crearConTresHabilidades();
    Personaje rival = crearConTresHabilidades();

    pj.recibirEleccion(1);
    std::optional<int> eleccion = pj.elegirHabilidad(propio, rival);

    REQUIRE(eleccion.has_value());
    CHECK(eleccion.value() == 1);
}

TEST_CASE("Jugador: la elección se entrega una sola vez (se olvida para el siguiente turno)")
{
    ControladorPJ pj;
    Personaje propio = crearConTresHabilidades();
    Personaje rival = crearConTresHabilidades();

    pj.recibirEleccion(2);
    CHECK(pj.elegirHabilidad(propio, rival).has_value());         // la entrega
    CHECK_FALSE(pj.elegirHabilidad(propio, rival).has_value());   // y ya no la repite
}

TEST_CASE("Jugador: si cambia de opinión antes de que se la pidan, vale la última")
{
    ControladorPJ pj;
    Personaje propio = crearConTresHabilidades();
    Personaje rival = crearConTresHabilidades();

    pj.recibirEleccion(0);
    pj.recibirEleccion(2);
    std::optional<int> eleccion = pj.elegirHabilidad(propio, rival);

    REQUIRE(eleccion.has_value());
    CHECK(eleccion.value() == 2);
}

TEST_CASE("Jugador: una elección fuera de rango se ignora")
{
    ControladorPJ pj;
    Personaje propio = crearConTresHabilidades();   // índices válidos: 0, 1, 2
    Personaje rival = crearConTresHabilidades();

    pj.recibirEleccion(5);
    CHECK_FALSE(pj.elegirHabilidad(propio, rival).has_value());

    pj.recibirEleccion(-1);
    CHECK_FALSE(pj.elegirHabilidad(propio, rival).has_value());
}

TEST_CASE("Jugador: funciona a través de un Controlador* (polimorfismo)")
{
    ControladorPJ pj;
    Controlador* controlador = &pj;    // Combate solo verá un Controlador*
    Personaje propio = crearConTresHabilidades();
    Personaje rival = crearConTresHabilidades();

    pj.recibirEleccion(0);
    std::optional<int> eleccion = controlador->elegirHabilidad(propio, rival);

    REQUIRE(eleccion.has_value());
    CHECK(eleccion.value() == 0);
}
