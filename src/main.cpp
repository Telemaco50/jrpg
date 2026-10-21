// Comprobación del entorno: abre una ventana, carga un PNG con SDL3_image
// y lo mueve con las flechas. Cuando empieces con tus clases, sustituye este archivo.

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

// Resolución "interna" tipo Game Boy Advance, escalada x3 en la ventana
constexpr int kAnchoLogico = 240;
constexpr int kAltoLogico  = 160;
constexpr int kEscala      = 3;

int main(int /*argc*/, char* /*argv*/[])   // parámetros sin nombre = "no los uso" (evita avisos)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error en SDL_Init: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow("JRPG",
                                           kAnchoLogico * kEscala,
                                           kAltoLogico * kEscala,
                                           0);
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, nullptr);
    if (!ventana || !renderer) {
        SDL_Log("Error creando ventana/renderer: %s", SDL_GetError());
        return 1;
    }

    // Dibujamos en coordenadas de 240x160 y SDL lo escala solo, sin difuminar (pixel art)
    SDL_SetRenderLogicalPresentation(renderer, kAnchoLogico, kAltoLogico,
                                     SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_PIXELART);

    // Cargar un PNG: si esto funciona, SDL3_image está bien enlazado
    SDL_Texture* sprite = IMG_LoadTexture(renderer, "assets/test.png");
    if (!sprite) {
        SDL_Log("No se pudo cargar assets/test.png: %s", SDL_GetError());
    }

    float x = 112.0f, y = 72.0f;          // posición del sprite
    const float velocidad = 60.0f;        // píxeles por SEGUNDO (no por frame)
    Uint64 anterior = SDL_GetTicksNS();

    bool ejecutando = true;
    while (ejecutando) {
        // Tiempo transcurrido desde el frame anterior, en segundos (delta time)
        Uint64 ahora = SDL_GetTicksNS();
        float dt = (ahora - anterior) / 1'000'000'000.0f;
        anterior = ahora;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) ejecutando = false;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) ejecutando = false;
        }

        // Estado actual del teclado (para movimiento continuo mientras se mantiene pulsada)
        const bool* teclas = SDL_GetKeyboardState(nullptr);
        if (teclas[SDL_SCANCODE_LEFT])  x -= velocidad * dt;
        if (teclas[SDL_SCANCODE_RIGHT]) x += velocidad * dt;
        if (teclas[SDL_SCANCODE_UP])    y -= velocidad * dt;
        if (teclas[SDL_SCANCODE_DOWN])  y += velocidad * dt;

        SDL_SetRenderDrawColor(renderer, 40, 120, 60, 255);   // verde "hierba"
        SDL_RenderClear(renderer);
        if (sprite) {
            SDL_FRect destino = { x, y, 16.0f, 16.0f };
            SDL_RenderTexture(renderer, sprite, nullptr, &destino);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(1);   // ceder la CPU; el movimiento no depende de esto gracias a dt
    }

    SDL_DestroyTexture(sprite);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
    return 0;
}
