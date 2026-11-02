// Selección de personaje y combate contra la IA

#include <SDL3/SDL_main.h>
#include <memory>
#include <random>
#include "interfaz.h"
#include "catalogo.h"
#include "seleccionPersonaje.h"
#include "combate.h"
#include "controladorPJ.h"
#include "controladorIA.h"

// Pantallas del juego
enum class Pantalla {
    Seleccion,
    Combate
};

int main(int /*argc*/, char* /*argv*/[])
{
    Interfaz interfaz;
    if(!interfaz.estaAbierta()){
        return 1;
    }

    std::random_device azar;

    SeleccionPersonaje seleccion(crearPersonajes());
    ControladorPJ controladorJugador;
    ControladorIA controladorRival(azar());
    std::unique_ptr<Combate> combate;
    int opcionMenu = 0;
    Uint64 inicioTurnoRival = 0;           // pausa del rival

    Pantalla pantalla = Pantalla::Seleccion;

    while(interfaz.estaAbierta()){
        // Eventos
        std::vector<Tecla> teclas = interfaz.leerTeclas();
        for(Tecla tecla : teclas){
            if(tecla == Tecla::Salir){
                return 0;
            }

            if(pantalla == Pantalla::Seleccion){
                if(tecla == Tecla::Izquierda) seleccion.moverIzquierda();
                if(tecla == Tecla::Derecha)   seleccion.moverDerecha();
                if(tecla == Tecla::Arriba)    seleccion.moverIzquierda();
                if(tecla == Tecla::Abajo)     seleccion.moverDerecha();
                if(tecla == Tecla::Aceptar){
                    seleccion.confirmar();
                    int rival = seleccion.elegirIndiceRival(azar());
                    combate = std::make_unique<Combate>(seleccion.getElegido(),
                                                        seleccion.getOpciones()[rival],
                                                        controladorJugador, controladorRival,
                                                        azar());
                    pantalla = Pantalla::Combate;
                }
            }
            else if(!combate->haTerminado() && combate->getTurno() == Lado::Jugador){
                int cantidad = static_cast<int>(combate->getJugador().getHabilidades().size());
                if(tecla == Tecla::Arriba) opcionMenu = (opcionMenu - 1 + cantidad) % cantidad;
                if(tecla == Tecla::Abajo)  opcionMenu = (opcionMenu + 1) % cantidad;
                if(tecla == Tecla::Aceptar){
                    controladorJugador.recibirEleccion(opcionMenu);
                }
            }
        }

        // Lógica
        if(pantalla == Pantalla::Combate && !combate->haTerminado()){
            if(combate->getTurno() == Lado::Jugador){
                combate->actualizar();
                inicioTurnoRival = SDL_GetTicks();
            }
            else if(SDL_GetTicks() - inicioTurnoRival > 800){
                combate->actualizar();
            }
        }

        // Dibujo
        interfaz.empezarFrame();
        if(pantalla == Pantalla::Seleccion){
            interfaz.dibujarSeleccion(seleccion);
        }else{
            interfaz.dibujarCombate(*combate, opcionMenu);
        }
        interfaz.terminarFrame();

        SDL_Delay(16);   // ~60 fps
    }

    return 0;
}
