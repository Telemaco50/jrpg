#include "catalogo.h"

std::vector<Personaje> crearPersonajes()
{

    std::vector<Personaje> lista;

    lista.push_back(Personaje("Alvaro",Tipo::Otaku,110,{
        Habilidad("Turno de magic infinito", Efecto::Canalizar, Escalado::noEscala, 99, 99, 0),
        Habilidad("Ingesta de ramen",        Efecto::Cura, Escalado::noEscala, 25, 25, 0),
        Habilidad("Hijo único",              Efecto::Dano, Escalado::noEscala, 20, 20, 0),
    },"alvaro"));

    lista.push_back(Personaje("Griso",Tipo::Correa,100,{
        Habilidad("Sermón monumental", Efecto::Stun, Escalado::turnoLibre,   1,  1, 0),
        Habilidad("Pásame la birra",   Efecto::Cura, Escalado::turnoLibre,  10, 10, 0),
        Habilidad("La maldita uni",       Efecto::Dano, Escalado::turnoCorrea, 10, 10, 0),
    },"griso"));



    
    lista.push_back(Personaje("Josemi",Tipo::Criptobro,85,{
        Habilidad("Desaparecer", Efecto::Stun, Escalado::noEscala,   1,  1, 0),
        Habilidad("Granja de bitcoin",   Efecto::Cura, Escalado::noEscala,  10, 15, 0),
        Habilidad("EL META",       Efecto::Dano, Escalado::noEscala, 30, 50, 0),
    },"josemi"));


    lista.push_back(Personaje("Josito", Tipo::Dormilon, 100, {
    Habilidad("Dependencia del bus", Efecto::Stun, Escalado::noEscala, 1,  3, 0),
    Habilidad("La yaya cocina",      Efecto::Cura, Escalado::noEscala, 5, 35, 0),
    Habilidad("EHHH",                Efecto::Dano, Escalado::noEscala, 5, 35, 0),
}, "josito"));

    return lista;
}