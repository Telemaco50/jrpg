// Pruebas de SeleccionPersonaje (solo la lógica de la pantalla de selección; no dibuja)

#include <doctest/doctest.h>
#include "seleccionPersonaje.h"
#include "catalogo.h"

TEST_CASE("Selección: empieza marcado el primero y sin confirmar")
{
    SeleccionPersonaje s(crearPersonajes());

    CHECK(s.getIndiceMarcado() == 0);
    CHECK_FALSE(s.haConfirmado());
    CHECK(s.getOpciones().size() == 4);
}

TEST_CASE("Selección: moverDerecha avanza la marca")
{
    SeleccionPersonaje s(crearPersonajes());

    s.moverDerecha();
    CHECK(s.getIndiceMarcado() == 1);
    s.moverDerecha();
    CHECK(s.getIndiceMarcado() == 2);
}

TEST_CASE("Selección: desde el último, moverDerecha vuelve al primero")
{
    SeleccionPersonaje s(crearPersonajes());

    for(int i = 0; i < 3; i++){
        s.moverDerecha();       // 0 → 1 → 2 → 3
    }
    CHECK(s.getIndiceMarcado() == 3);
    s.moverDerecha();           // 3 → 0 (da la vuelta)
    CHECK(s.getIndiceMarcado() == 0);
}

TEST_CASE("Selección: desde el primero, moverIzquierda va al último")
{
    SeleccionPersonaje s(crearPersonajes());

    s.moverIzquierda();         // 0 → 3 (da la vuelta)
    CHECK(s.getIndiceMarcado() == 3);
    s.moverIzquierda();         // 3 → 2
    CHECK(s.getIndiceMarcado() == 2);
}

TEST_CASE("Selección: confirmar elige el personaje marcado")
{
    std::vector<Personaje> catalogo = crearPersonajes();
    SeleccionPersonaje s(catalogo);

    s.moverDerecha();
    s.moverDerecha();
    s.confirmar();

    CHECK(s.haConfirmado());
    CHECK(s.getElegido().getNombre() == catalogo[2].getNombre());
}

TEST_CASE("Selección: después de confirmar, la marca ya no se mueve")
{
    SeleccionPersonaje s(crearPersonajes());

    s.moverDerecha();           // marcado 1
    s.confirmar();
    s.moverDerecha();
    s.moverIzquierda();
    s.moverIzquierda();

    CHECK(s.getIndiceMarcado() == 1);
}

TEST_CASE("Selección: el rival nunca es el mismo personaje y es un índice válido")
{
    for(int marcado = 0; marcado < 4; marcado++){
        SeleccionPersonaje s(crearPersonajes());
        for(int i = 0; i < marcado; i++){
            s.moverDerecha();
        }
        s.confirmar();

        for(unsigned int semilla = 0; semilla < 50; semilla++){
            int rival = s.elegirIndiceRival(semilla);
            CAPTURE(marcado);
            CAPTURE(semilla);
            CHECK(rival != marcado);
            CHECK(rival >= 0);
            CHECK(rival < 4);
        }
    }
}

TEST_CASE("Selección: el rival puede ser cualquiera de los otros tres")
{
    SeleccionPersonaje s(crearPersonajes());    // marcado el 0
    s.confirmar();

    bool salio[4] = {false, false, false, false};
    for(unsigned int semilla = 0; semilla < 100; semilla++){
        salio[s.elegirIndiceRival(semilla)] = true;
    }

    CHECK_FALSE(salio[0]);      // nunca él mismo
    CHECK(salio[1]);
    CHECK(salio[2]);
    CHECK(salio[3]);
}

TEST_CASE("Selección: la misma semilla da el mismo rival")
{
    SeleccionPersonaje s(crearPersonajes());
    s.confirmar();

    CHECK(s.elegirIndiceRival(7) == s.elegirIndiceRival(7));
}
