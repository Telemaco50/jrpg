// Punto de entrada del juego: une todas las piezas.
//   1. Pantalla de selección (SeleccionPersonaje)
//   2. Combate: el jugador con el menú (ControladorPJ) contra la IA (ControladorIA)
//   3. Resultado

#include <SDL3/SDL_main.h>
#include <memory>
#include <random>
#include "interfaz.h"
#include "catalogo.h"
#include "seleccionPersonaje.h"
#include "combate.h"
#include "controladorPJ.h"
#include "controladorIA.h"

// Las fases del juego (una "máquina de estados" muy sencilla)
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

    // Semilla distinta en cada partida (en las pruebas usamos una fija)
    std::random_device azar;

    SeleccionPersonaje seleccion(crearPersonajes());
    ControladorPJ controladorJugador;
    ControladorIA controladorRival(azar());
    std::unique_ptr<Combate> combate;      // vacío hasta que se elija personaje
    int opcionMenu = 0;                    // habilidad marcada en el menú del combate
    Uint64 inicioTurnoRival = 0;           // para que el rival "piense" un momento

    Pantalla pantalla = Pantalla::Seleccion;

    while(interfaz.estaAbierta()){
        // ---------- 1. EVENTOS ----------
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

        // ---------- 2. LÓGICA ----------
        if(pantalla == Pantalla::Combate && !combate->haTerminado()){
            if(combate->getTurno() == Lado::Jugador){
                combate->actualizar();
                inicioTurnoRival = SDL_GetTicks();
            }
            else if(SDL_GetTicks() - inicioTurnoRival > 800){   // el rival "piensa" 0,8 s
                combate->actualizar();
            }
        }

        // ---------- 3. DIBUJAR ----------
        interfaz.empezarFrame();
        if(pantalla == Pantalla::Seleccion){
            interfaz.dibujarSeleccion(seleccion);
        }else{
            interfaz.dibujarCombate(*combate, opcionMenu);
        }
        interfaz.terminarFrame();

        SDL_Delay(16);   // ~60 frames por segundo
    }

    return 0;
}
