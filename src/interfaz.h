#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>
#include <vector>

#include "seleccionPersonaje.h"
#include "combate.h"

// Teclas que usa el juego
enum class Tecla {
    Izquierda,
    Derecha,
    Arriba,
    Abajo,
    Aceptar,    // Enter o Espacio
    Salir       // Escape
};

// Única clase que usa SDL: ventana, teclado y dibujo de las pantallas.
// Resolución lógica de 240x160 ampliada en la ventana; (0, 0) es la esquina superior izquierda.
class Interfaz {
public:
    static constexpr int kAncho = 240;
    static constexpr int kAlto = 160;

    // RAII: el constructor crea los recursos y el destructor los libera
    Interfaz();
    ~Interfaz();

    // No copiable: es dueña de recursos de SDL
    Interfaz(const Interfaz&) = delete;
    Interfaz& operator=(const Interfaz&) = delete;

    // false si se ha cerrado la ventana o falló el arranque
    bool estaAbierta() const {
        return m_abierta;
    }

    // Teclas pulsadas en este frame (una llamada por frame)
    std::vector<Tecla> leerTeclas();

    // --- Pantallas ---
    void dibujarSeleccion(const SeleccionPersonaje& seleccion);
    void dibujarCombate(const Combate& combate, int opcionMenu);

    // --- Primitivas de dibujo ---
    void empezarFrame();          // borra la pantalla
    void terminarFrame();         // muestra en la ventana lo dibujado

    void dibujarTexto(const std::string& texto, float x, float y,
                      SDL_Color color = {255, 255, 255, 255});
    int anchoTexto(const std::string& texto);    // en píxeles

    // Parte el texto en varias líneas si no cabe en anchoMaximo
    void dibujarTextoAjustado(const std::string& texto, float x, float y, int anchoMaximo,
                              SDL_Color color = {255, 255, 255, 255});

    // assets/sprites/<nombreSprite>_frente.png o _espalda.png (escala 1 = 64x64)
    void dibujarSprite(const std::string& nombreSprite, bool deFrente,
                       float x, float y, float escala = 1.0f);

    void dibujarRectangulo(float x, float y, float ancho, float alto,
                           SDL_Color color, bool relleno = true);

private:
    // Carga cada imagen una sola vez (caché)
    SDL_Texture* cargarTextura(const std::string& ruta);

    // Correa, aturdido, canalizando...
    void dibujarEstado(const Personaje& personaje, float x, float y);

    bool m_abierta = false;
    SDL_Window* m_ventana = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    TTF_Font* m_fuente = nullptr;

    // Caché de texturas por ruta
    std::map<std::string, SDL_Texture*> m_texturas;
};
