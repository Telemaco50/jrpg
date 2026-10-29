#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>
#include <vector>

#include "seleccionPersonaje.h"
#include "combate.h"

// Las teclas que le importan al juego (traducidas desde SDL)
enum class Tecla {
    Izquierda,
    Derecha,
    Arriba,
    Abajo,
    Aceptar,    // Enter o Espacio
    Salir       // Escape
};

// La ÚNICA clase del juego que habla con SDL.
//
// Se divide en dos partes:
//   1. FONTANERÍA (ya hecha): crear y destruir la ventana, la fuente y las texturas,
//      leer el teclado, y "primitivas" para dibujar: texto, sprites y rectángulos.
//   2. PANTALLAS (tu parte): dibujarSeleccion() y dibujarCombate() deciden DÓNDE y CÓMO
//      se ve cada cosa, usando las primitivas. Ahora tienen una versión provisional solo con texto.
//
// Coordenadas: la pantalla mide 240 x 160 píxeles (como una Game Boy Advance) y SDL la amplía
// sola en la ventana. (0, 0) es la esquina de ARRIBA a la izquierda; la Y crece hacia ABAJO.
class Interfaz {
public:
    static constexpr int kAncho = 240;
    static constexpr int kAlto = 160;

    // RAII: el constructor lo crea todo y el destructor lo libera todo
    Interfaz();
    ~Interfaz();

    // Esta clase es dueña de recursos de SDL (punteros que hay que liberar): si se copiara,
    // dos objetos intentarían liberar lo mismo. Así que se prohíbe copiarla.
    Interfaz(const Interfaz&) = delete;
    Interfaz& operator=(const Interfaz&) = delete;

    // ¿Sigue abierta la ventana? (false si el jugador la cierra o si algo falló al arrancar)
    bool estaAbierta() const {
        return m_abierta;
    }

    // Lee los eventos de SDL de este frame y devuelve las teclas pulsadas.
    // Llamarla UNA vez por frame (es el único sitio del juego que lee eventos).
    std::vector<Tecla> leerTeclas();

    // --- Pantallas (TU PARTE) ---
    void dibujarSeleccion(const SeleccionPersonaje& seleccion);
    void dibujarCombate(const Combate& combate, int opcionMenu);

    // --- Primitivas de dibujo (fontanería, para usarlas en las pantallas) ---
    void empezarFrame();          // borra la pantalla
    void terminarFrame();         // muestra en la ventana lo dibujado

    void dibujarTexto(const std::string& texto, float x, float y,
                      SDL_Color color = {255, 255, 255, 255});
    int anchoTexto(const std::string& texto);    // cuántos píxeles ocupa (para centrar)

    // Dibuja assets/sprites/<nombreSprite>_frente.png (o _espalda.png).
    // escala 1 = 64x64; 0.5 = 32x32; 2 = 128x128...
    void dibujarSprite(const std::string& nombreSprite, bool deFrente,
                       float x, float y, float escala = 1.0f);

    void dibujarRectangulo(float x, float y, float ancho, float alto,
                           SDL_Color color, bool relleno = true);

private:
    // Carga una imagen la primera vez que se pide y la guarda para las siguientes (caché)
    SDL_Texture* cargarTextura(const std::string& ruta);

    bool m_abierta = false;
    SDL_Window* m_ventana = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    TTF_Font* m_fuente = nullptr;

    // Caché de imágenes: ruta → textura (como una tabla de búsqueda: SELECT textura WHERE ruta = ...)
    std::map<std::string, SDL_Texture*> m_texturas;
};
