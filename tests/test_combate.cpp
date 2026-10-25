// Pruebas de Combate (reglas en docs/DISENO.md, sección "Reglas del combate")
//
// Truco: los dos bandos se controlan con ControladorPJ, así la prueba decide qué hace
// cada uno en cada turno (como si fueran dos jugadores pulsando el menú).
// Las habilidades tienen mínimo == máximo para que las cantidades sean exactas.
//
// Tabla de efectividad usada aquí (ataca → defiende):
//   Otaku → Criptobro x1   Otaku → Narcolepsia x2   Criptobro → Narcolepsia x0.5
//   Correa → Narcolepsia x1

#include <doctest/doctest.h>
#include "combate.h"
#include "controladorPJ.h"

// --- Ayudas para crear habilidades y personajes de prueba ---
static Habilidad golpe(int cantidad)
{
    return Habilidad("Golpe", Efecto::Dano, Escalado::noEscala, cantidad, cantidad, 0);
}

static Habilidad sanar(int cantidad)
{
    return Habilidad("Sanar", Efecto::Cura, Escalado::noEscala, cantidad, cantidad, 0);
}

static Habilidad paralizar(int turnos)
{
    return Habilidad("Paralizar", Efecto::Stun, Escalado::noEscala, turnos, turnos, 0);
}

static Habilidad noHacerNada()
{
    return Habilidad("Nada", Efecto::Cura, Escalado::noEscala, 0, 0, 0);
}

static Personaje crear(const std::string& nombre, Tipo tipo, int ps, const std::vector<Habilidad>& habilidades)
{
    return Personaje(nombre, tipo, ps, habilidades, nombre);
}

// ============================================================================

TEST_CASE("Combate: empieza el jugador y no ha terminado")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}),
              crear("B", Tipo::Criptobro, 100, {golpe(10)}), cj, cr, 1);

    CHECK(c.getTurno() == Lado::Jugador);
    CHECK_FALSE(c.haTerminado());
    CHECK_FALSE(c.getGanador().has_value());
}

TEST_CASE("Combate: si el jugador no ha elegido, actualizar() no hace nada")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}),
              crear("B", Tipo::Criptobro, 100, {golpe(10)}), cj, cr, 1);

    c.actualizar();
    c.actualizar();

    CHECK(c.getTurno() == Lado::Jugador);
    CHECK(c.getRival().getPsActuales() == 100);
}

TEST_CASE("Combate: daño normal (x1) y el turno pasa al rival")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}),
              crear("B", Tipo::Criptobro, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();

    CHECK(c.getRival().getPsActuales() == 90);
    CHECK(c.getTurno() == Lado::Rival);
}

TEST_CASE("Combate: el rival también ataca en su turno")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}),
              crear("B", Tipo::Criptobro, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();             // jugador ataca
    cr.recibirEleccion(0);
    c.actualizar();             // rival ataca (Criptobro → Otaku x1)

    CHECK(c.getJugador().getPsActuales() == 90);
    CHECK(c.getTurno() == Lado::Jugador);
}

TEST_CASE("Combate: daño muy eficaz (x2)")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}),
              crear("B", Tipo::Narcolepsia, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();

    CHECK(c.getRival().getPsActuales() == 80);
}

TEST_CASE("Combate: daño poco eficaz (x0.5)")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Criptobro, 100, {golpe(10)}),
              crear("B", Tipo::Narcolepsia, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();

    CHECK(c.getRival().getPsActuales() == 95);
}

TEST_CASE("Combate: el daño con decimales se redondea hacia abajo")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Criptobro, 100, {golpe(15)}),     // 15 x 0.5 = 7.5 → 7
              crear("B", Tipo::Narcolepsia, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();

    CHECK(c.getRival().getPsActuales() == 93);
}

TEST_CASE("Combate: la cura se la aplica el propio personaje")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(30)}),
              crear("B", Tipo::Criptobro, 100, {sanar(20)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();             // rival: 100 - 30 = 70
    cr.recibirEleccion(0);
    c.actualizar();             // rival se cura: 70 + 20 = 90

    CHECK(c.getRival().getPsActuales() == 90);
    CHECK(c.getJugador().getPsActuales() == 100);   // la cura no afecta al otro
}

TEST_CASE("Combate: aturdir hace perder el turno al rival")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {paralizar(1)}),
              crear("B", Tipo::Criptobro, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();             // el jugador aturde al rival 1 turno
    CHECK(c.getRival().estaAturdido());
    CHECK(c.getTurno() == Lado::Rival);

    c.actualizar();             // turno del rival: está aturdido, lo pierde SIN elegir nada
    CHECK(c.getTurno() == Lado::Jugador);
    CHECK_FALSE(c.getRival().estaAturdido());
    CHECK(c.getJugador().getPsActuales() == 100);   // el rival no ha podido atacar
}

TEST_CASE("Combate: gana quien deja al otro a 0 PS")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}),
              crear("B", Tipo::Criptobro, 10, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();

    CHECK(c.haTerminado());
    REQUIRE(c.getGanador().has_value());
    CHECK(c.getGanador().value() == Lado::Jugador);
}

TEST_CASE("Combate: cuando ha terminado, actualizar() ya no hace nada")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}),
              crear("B", Tipo::Criptobro, 10, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();             // gana el jugador
    cr.recibirEleccion(0);
    c.actualizar();             // el rival ya no puede atacar

    CHECK(c.getJugador().getPsActuales() == 100);
}

TEST_CASE("Combate: los personajes originales no se modifican (el combate usa copias)")
{
    ControladorPJ cj, cr;
    Personaje original = crear("B", Tipo::Criptobro, 100, {golpe(10)});
    Combate c(crear("A", Tipo::Otaku, 100, {golpe(10)}), original, cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();

    CHECK(c.getRival().getPsActuales() == 90);
    CHECK(original.getPsActuales() == 100);
}

TEST_CASE("Combate: la cantidad aleatoria respeta el mínimo y el máximo")
{
    ControladorPJ cj, cr;
    Combate c(crear("A", Tipo::Otaku, 100,
                    {Habilidad("Aleatorio", Efecto::Dano, Escalado::noEscala, 5, 15, 0)}),
              crear("B", Tipo::Criptobro, 100000, {noHacerNada()}), cj, cr, 42);

    bool salioMenorQue10 = false;
    bool salioMayorQue10 = false;

    for(int i = 0; i < 50; i++){
        int antes = c.getRival().getPsActuales();
        cj.recibirEleccion(0);
        c.actualizar();         // jugador ataca (x1)
        int danio = antes - c.getRival().getPsActuales();

        CHECK(danio >= 5);
        CHECK(danio <= 15);
        if(danio < 10) salioMenorQue10 = true;
        if(danio > 10) salioMayorQue10 = true;

        cr.recibirEleccion(0);
        c.actualizar();         // rival no hace nada
    }

    CHECK(salioMenorQue10);     // de verdad es aleatorio (no siempre lo mismo)
    CHECK(salioMayorQue10);
}

TEST_CASE("Combate: escalado de Griso con la correa (multiplicador = max(1, turnos con correa))")
{
    ControladorPJ cj, cr;
    Combate c(crear("Griso", Tipo::Correa, 100,
                    {Habilidad("Uni", Efecto::Dano, Escalado::turnoCorrea, 10, 10, 0)}),
              crear("B", Tipo::Narcolepsia, 1000, {noHacerNada()}), cj, cr, 1);   // Correa → Narcolepsia x1

    // Cada ronda: Griso ataca con "Uni" y el rival no hace nada
    auto ronda = [&]() {
        cj.recibirEleccion(0);
        c.actualizar();
        cr.recibirEleccion(0);
        c.actualizar();
    };

    ronda();    // turnos con correa 0 → x1 → 10   (después: 1)
    CHECK(c.getRival().getPsActuales() == 990);
    ronda();    // turnos con correa 1 → x1 → 10   (después: 2)
    CHECK(c.getRival().getPsActuales() == 980);
    ronda();    // turnos con correa 2 → x2 → 20   (después: 3 → se libera, contadores a 0)
    CHECK(c.getRival().getPsActuales() == 960);
    ronda();    // libre: turnos con correa 0 → x1 → 10
    CHECK(c.getRival().getPsActuales() == 950);
}

// ============================================================================
// Canalización ("Turno de magic infinito" de Alvaro)
// ============================================================================

static Habilidad canalizar(int turnos)
{
    return Habilidad("Magic infinito", Efecto::Canalizar, Escalado::noEscala, turnos, turnos, 0);
}

TEST_CASE("Combate: si nadie interrumpe, la canalización aturde 99 turnos al rival")
{
    ControladorPJ cj, cr;
    Combate c(crear("Alvaro", Tipo::Otaku, 100, {canalizar(99), golpe(10)}),
              crear("B", Tipo::Criptobro, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();             // Alvaro empieza a canalizar
    CHECK(c.getJugador().estaCanalizando());
    CHECK(c.getTurno() == Lado::Rival);

    cr.recibirEleccion(0);
    c.actualizar();             // el rival le pega (el daño NO interrumpe)
    CHECK(c.getJugador().estaCanalizando());

    c.actualizar();             // turno de Alvaro: se completa sola (sin elegir nada) y gasta el turno
    CHECK_FALSE(c.getJugador().estaCanalizando());
    CHECK(c.getRival().getTurnosAturdido() == 99);
    CHECK(c.getTurno() == Lado::Rival);
}

TEST_CASE("Combate: tras completar la canalización, el rival no vuelve a actuar")
{
    ControladorPJ cj, cr;
    Combate c(crear("Alvaro", Tipo::Otaku, 100, {canalizar(99), golpe(10)}),
              crear("B", Tipo::Criptobro, 100, {golpe(10)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();             // canaliza
    cr.recibirEleccion(0);
    c.actualizar();             // rival pega: Alvaro 90
    c.actualizar();             // se completa: rival aturdido 99

    for(int i = 0; i < 5; i++){
        cr.recibirEleccion(0);  // aunque el rival "quiera" atacar...
        c.actualizar();         // ...está aturdido y pierde el turno
        cj.recibirEleccion(1);
        c.actualizar();         // Alvaro golpea
    }

    CHECK(c.getJugador().getPsActuales() == 90);    // el rival no ha vuelto a pegar
    CHECK(c.getRival().getPsActuales() == 50);      // 5 golpes de 10
}

TEST_CASE("Combate: aturdir a Alvaro mientras canaliza la rompe y le hace perder el turno")
{
    ControladorPJ cj, cr;
    Combate c(crear("Alvaro", Tipo::Otaku, 100, {canalizar(99), golpe(10)}),
              crear("B", Tipo::Criptobro, 100, {paralizar(1)}), cj, cr, 1);

    cj.recibirEleccion(0);
    c.actualizar();             // Alvaro canaliza
    cr.recibirEleccion(0);
    c.actualizar();             // el rival le aturde → canalización rota
    CHECK_FALSE(c.getJugador().estaCanalizando());
    CHECK(c.getJugador().estaAturdido());

    c.actualizar();             // turno de Alvaro: está aturdido, lo pierde
    CHECK(c.getTurno() == Lado::Rival);
    CHECK_FALSE(c.getRival().estaAturdido());       // la canalización no llegó a lanzarse
}
