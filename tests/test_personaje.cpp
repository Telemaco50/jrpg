#include <doctest/doctest.h>
#include "personaje.h"

//Personaje para recibir las pruebas
static Personaje crearPersonajeP(){

    return Personaje("Prueba",Tipo::Otaku,100,{},"prueba");



}

TEST_CASE("Empieza con la vida a 100")
{
    Personaje Personajep=crearPersonajeP();
 CHECK(Personajep.getPsActuales()==100);



}

TEST_CASE("Recibir daion resta 30 de vida")
{
    Personaje Personajep=crearPersonajeP();
    Personajep.recibirDanio(30);
    CHECK(Personajep.getPsActuales()==70);



}

TEST_CASE("El daño no baja de 0")
{
    Personaje Personajep=crearPersonajeP();
    Personajep.recibirDanio(999);
    CHECK(Personajep.getPsActuales()==0);



}


TEST_CASE("Curar funciona")
{
    Personaje Personajep=crearPersonajeP();
    Personajep.recibirDanio(50);
    Personajep.curar(20);
    CHECK(Personajep.getPsActuales()==70);

}

TEST_CASE("Curar no pasa del limite")
{
    Personaje Personajep=crearPersonajeP();
    Personajep.curar(999);
    CHECK(Personajep.getPsActuales()==Personajep.getPsMax());

}

TEST_CASE("Check de vida esta funciona")
{
    Personaje Personajep=crearPersonajeP();
    CHECK(Personajep.estaVivo()==true);
    Personajep.recibirDanio(999);
    CHECK(Personajep.estaVivo()==false);
}

TEST_CASE("Aturdir funciona")
{
    Personaje Personajep=crearPersonajeP();
    Personajep.aturdir(2);
    CHECK(Personajep.estaAturdido()==true);

}

TEST_CASE("Esta aturdido se gasta")
{
    Personaje Personajep=crearPersonajeP();
    Personajep.aturdir(1);
    Personajep.pasarTurno();
    CHECK(Personajep.estaAturdido()==false);

}

// ============================================================================
// Pruebas añadidas: casos límite que las pruebas anteriores no cubrían
// ============================================================================

// Un personaje con un máximo distinto de 100, para pillar números escritos "a fuego"
static Personaje crearPersonaje50()
{
    return Personaje("Prueba50", Tipo::Narcolepsia, 50, {}, "prueba50");
}

TEST_CASE("La cura respeta el máximo de cada personaje (no un 100 fijo)")
{
    Personaje p = crearPersonaje50();
    p.recibirDanio(10);                 // 40
    p.curar(30);                        // 70... pero su máximo es 50
    CHECK(p.getPsActuales() == 50);
}

TEST_CASE("La cura no pasa del máximo partiendo de vida no llena")
{
    Personaje p = crearPersonajeP();
    p.recibirDanio(10);                 // 90
    p.curar(50);                        // 140... pero el máximo es 100
    CHECK(p.getPsActuales() == 100);
}

TEST_CASE("El aturdimiento se gasta de uno en uno")
{
    Personaje p = crearPersonajeP();
    p.aturdir(2);
    p.pasarTurno();
    CHECK(p.estaAturdido());            // le queda 1 turno
    p.pasarTurno();
    CHECK_FALSE(p.estaAturdido());      // ya no
}

TEST_CASE("Pasar turno sin estar aturdido no deja el contador en negativo")
{
    Personaje p = crearPersonajeP();
    p.pasarTurno();
    p.pasarTurno();                     // dos turnos normales, sin aturdir
    CHECK(p.getTurnosAturdido() == 0);
    p.aturdir(1);
    CHECK(p.estaAturdido());            // el aturdimiento tiene que funcionar
}

TEST_CASE("El aturdimiento se acumula")
{
    Personaje p = crearPersonajeP();
    p.aturdir(2);
    p.aturdir(1);
    CHECK(p.getTurnosAturdido() == 3);
}

// ============================================================================
// Correa de Griso (regla en docs/DISENO.md): empieza con correa y cambia de
// estado cada 3 turnos. Al cambiar, los contadores vuelven a 0.
// ============================================================================

static Personaje crearCorreaP()
{
    return Personaje("GrisoPrueba", Tipo::Correa, 100, {}, "griso");
}

// Llama a pasarTurno() n veces
static void pasarTurnos(Personaje& p, int n)
{
    for (int i = 0; i < n; i++) {
        p.pasarTurno();
    }
}

TEST_CASE("Correa: empieza con correa y los contadores a 0")
{
    Personaje p = crearCorreaP();
    CHECK(p.tieneCorrea());
    CHECK(p.turnosConCorrea() == 0);
    CHECK(p.turnosLibre() == 0);
}

TEST_CASE("Correa: cuenta los turnos con correa")
{
    Personaje p = crearCorreaP();
    pasarTurnos(p, 2);
    CHECK(p.tieneCorrea());
    CHECK(p.turnosConCorrea() == 2);
    CHECK(p.turnosLibre() == 0);
}

TEST_CASE("Correa: se libera a los 3 turnos y el contador vuelve a 0")
{
    Personaje p = crearCorreaP();
    pasarTurnos(p, 3);
    CHECK_FALSE(p.tieneCorrea());
    CHECK(p.turnosConCorrea() == 0);
    CHECK(p.turnosLibre() == 0);
}

TEST_CASE("Correa: cuenta los turnos libre")
{
    Personaje p = crearCorreaP();
    pasarTurnos(p, 3 + 2);
    CHECK_FALSE(p.tieneCorrea());
    CHECK(p.turnosLibre() == 2);
    CHECK(p.turnosConCorrea() == 0);
}

TEST_CASE("Correa: vuelve a ponérsela a los 3 turnos libre")
{
    Personaje p = crearCorreaP();
    pasarTurnos(p, 6);
    CHECK(p.tieneCorrea());
    CHECK(p.turnosConCorrea() == 0);
    CHECK(p.turnosLibre() == 0);
}

TEST_CASE("Correa: los personajes de otros tipos no usan la correa")
{
    Personaje p = crearPersonajeP();    // tipo Otaku
    pasarTurnos(p, 5);
    CHECK_FALSE(p.tieneCorrea());
    CHECK(p.turnosConCorrea() == 0);
    CHECK(p.turnosLibre() == 0);
}
