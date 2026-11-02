// Pruebas del catálogo de personajes
#include <doctest/doctest.h>
#include "catalogo.h"

// Busca un personaje por nombre en la lista (nullptr si no está)
static const Personaje* buscar(const std::vector<Personaje>& lista, const std::string& nombre)
{
    for(const Personaje& p : lista){
        if(p.getNombre() == nombre){
            return &p;
        }
    }
    return nullptr;
}

TEST_CASE("Catálogo: hay 4 personajes")
{
    CHECK(crearPersonajes().size() == 4);
}

TEST_CASE("Catálogo: cada personaje tiene su tipo")
{
    std::vector<Personaje> lista = crearPersonajes();

    const Personaje* alvaro = buscar(lista, "Alvaro");
    const Personaje* josemi = buscar(lista, "Josemi");
    const Personaje* josito = buscar(lista, "Josito");
    const Personaje* griso  = buscar(lista, "Griso");

    REQUIRE(alvaro != nullptr);
    REQUIRE(josemi != nullptr);
    REQUIRE(josito != nullptr);
    REQUIRE(griso  != nullptr);

    CHECK(alvaro->getTipo() == Tipo::Otaku);
    CHECK(josemi->getTipo() == Tipo::Criptobro);
    CHECK(josito->getTipo() == Tipo::Dormilon);
    CHECK(griso->getTipo()  == Tipo::Correa);
}

TEST_CASE("Catálogo: todos tienen 3 habilidades, PS positivos y empiezan con la vida llena")
{
    for(const Personaje& p : crearPersonajes()){
        CAPTURE(p.getNombre());
        CHECK(p.getHabilidades().size() == 3);
        CHECK(p.getPsMax() > 0);
        CHECK(p.getPsActuales() == p.getPsMax());
    }
}

TEST_CASE("Catálogo: en todas las habilidades el mínimo no supera al máximo")
{
    for(const Personaje& p : crearPersonajes()){
        for(const Habilidad& h : p.getHabilidades()){
            CAPTURE(h.getNombre());
            CHECK(h.getCantidadMin() <= h.getCantidadMax());
            CHECK(h.getCantidadMin() >= 0);
        }
    }
}

TEST_CASE("Catálogo: cada personaje tiene una habilidad de cada efecto que le toca")
{
    for(const Personaje& p : crearPersonajes()){
        CAPTURE(p.getNombre());
        int curas = 0, danios = 0, controles = 0;
        for(const Habilidad& h : p.getHabilidades()){
            if(h.getEfecto() == Efecto::Cura) curas++;
            if(h.getEfecto() == Efecto::Dano) danios++;
            if(h.getEfecto() == Efecto::Stun || h.getEfecto() == Efecto::Canalizar) controles++;
        }
        CHECK(curas == 1);
        CHECK(danios == 1);
        CHECK(controles == 1);
    }
}

TEST_CASE("Catálogo: solo Griso (tipo Correa) tiene habilidades que escalan")
{
    for(const Personaje& p : crearPersonajes()){
        for(const Habilidad& h : p.getHabilidades()){
            CAPTURE(p.getNombre());
            CAPTURE(h.getNombre());
            if(p.getTipo() != Tipo::Correa){
                CHECK(h.getEscalado() == Escalado::noEscala);
            }
        }
    }
}

TEST_CASE("Catálogo: la habilidad de Alvaro es la canalización de 99 turnos")
{
    std::vector<Personaje> lista = crearPersonajes();
    const Personaje* alvaro = buscar(lista, "Alvaro");
    REQUIRE(alvaro != nullptr);

    bool tieneCanalizacion = false;
    for(const Habilidad& h : alvaro->getHabilidades()){
        if(h.getEfecto() == Efecto::Canalizar){
            tieneCanalizacion = true;
            CHECK(h.getCantidadMin() == 99);
        }
    }
    CHECK(tieneCanalizacion);
}

TEST_CASE("Catálogo: crearPersonajes() devuelve copias nuevas cada vez")
{
    std::vector<Personaje> partida1 = crearPersonajes();
    partida1[0].recibirDanio(50);

    std::vector<Personaje> partida2 = crearPersonajes();
    CHECK(partida2[0].getPsActuales() == partida2[0].getPsMax());   // la nueva partida empieza entera
}
