#include "interfaz.h"
#include <SDL3_image/SDL_image.h>

// ============================================================================
// 1. FONTANERÍA: crear, destruir, teclado y primitivas de dibujo
// ============================================================================

Interfaz::Interfaz()
{
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Error en SDL_Init: %s", SDL_GetError());
        return;
    }
    if(!TTF_Init()){
        SDL_Log("Error en TTF_Init: %s", SDL_GetError());
        return;
    }

    // Ventana 4 veces más grande que la pantalla lógica (240x160 → 960x640)
    m_ventana = SDL_CreateWindow("JRPG", kAncho * 4, kAlto * 4, 0);
    m_renderer = SDL_CreateRenderer(m_ventana, nullptr);
    if(!m_ventana || !m_renderer){
        SDL_Log("Error creando la ventana: %s", SDL_GetError());
        return;
    }

    // Dibujamos en coordenadas de 240x160 y SDL amplía sin difuminar (pixel art)
    SDL_SetRenderLogicalPresentation(m_renderer, kAncho, kAlto,
                                     SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    SDL_SetDefaultTextureScaleMode(m_renderer, SDL_SCALEMODE_PIXELART);

    // Fuente pixel art a 8 píxeles (Press Start 2P está diseñada en una cuadrícula de 8x8)
    m_fuente = TTF_OpenFont("assets/fuentes/PressStart2P-Regular.ttf", 8);
    if(!m_fuente){
        SDL_Log("No se pudo cargar la fuente: %s", SDL_GetError());
        return;
    }

    m_abierta = true;
}

Interfaz::~Interfaz()
{
    // Liberar en orden inverso a la creación
    for(auto& [ruta, textura] : m_texturas){
        SDL_DestroyTexture(textura);
    }
    if(m_fuente)   TTF_CloseFont(m_fuente);
    if(m_renderer) SDL_DestroyRenderer(m_renderer);
    if(m_ventana)  SDL_DestroyWindow(m_ventana);
    TTF_Quit();
    SDL_Quit();
}

std::vector<Tecla> Interfaz::leerTeclas()
{
    std::vector<Tecla> teclas;

    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_EVENT_QUIT){
            m_abierta = false;
        }
        // e.key.repeat: ignorar la repetición automática al mantener la tecla pulsada
        if(e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat){
            switch(e.key.key){
                case SDLK_LEFT:   teclas.push_back(Tecla::Izquierda); break;
                case SDLK_RIGHT:  teclas.push_back(Tecla::Derecha);   break;
                case SDLK_UP:     teclas.push_back(Tecla::Arriba);    break;
                case SDLK_DOWN:   teclas.push_back(Tecla::Abajo);     break;
                case SDLK_RETURN:
                case SDLK_SPACE:  teclas.push_back(Tecla::Aceptar);   break;
                case SDLK_ESCAPE: teclas.push_back(Tecla::Salir);     break;
                default: break;
            }
        }
    }
    return teclas;
}

void Interfaz::empezarFrame()
{
    SDL_SetRenderDrawColor(m_renderer, 20, 20, 40, 255);   // fondo azul oscuro
    SDL_RenderClear(m_renderer);
}

void Interfaz::terminarFrame()
{
    SDL_RenderPresent(m_renderer);
}

void Interfaz::dibujarTexto(const std::string& texto, float x, float y, SDL_Color color)
{
    if(texto.empty() || !m_fuente){
        return;
    }
    // Receta del texto: escribirlo en una imagen → convertirla en textura → pegarla
    SDL_Surface* superficie = TTF_RenderText_Solid(m_fuente, texto.c_str(), 0, color);
    if(!superficie){
        return;
    }
    SDL_Texture* textura = SDL_CreateTextureFromSurface(m_renderer, superficie);
    SDL_DestroySurface(superficie);
    if(!textura){
        return;
    }

    float ancho = 0, alto = 0;
    SDL_GetTextureSize(textura, &ancho, &alto);
    SDL_FRect destino = {x, y, ancho, alto};
    SDL_RenderTexture(m_renderer, textura, nullptr, &destino);

    // Esta textura solo vale para este texto y este frame: se destruye ya
    SDL_DestroyTexture(textura);
}

int Interfaz::anchoTexto(const std::string& texto)
{
    int ancho = 0, alto = 0;
    if(m_fuente){
        TTF_GetStringSize(m_fuente, texto.c_str(), 0, &ancho, &alto);
    }
    return ancho;
}

SDL_Texture* Interfaz::cargarTextura(const std::string& ruta)
{
    // ¿Ya la tenemos cargada? Entonces se reutiliza (cargar un PNG es lento)
    auto encontrada = m_texturas.find(ruta);
    if(encontrada != m_texturas.end()){
        return encontrada->second;
    }

    SDL_Texture* textura = IMG_LoadTexture(m_renderer, ruta.c_str());
    if(!textura){
        SDL_Log("No se pudo cargar %s: %s", ruta.c_str(), SDL_GetError());
    }
    m_texturas[ruta] = textura;   // se guarda aunque sea nullptr, para no reintentarlo cada frame
    return textura;
}

void Interfaz::dibujarSprite(const std::string& nombreSprite, bool deFrente,
                             float x, float y, float escala)
{
    std::string ruta = "assets/sprites/" + nombreSprite + (deFrente ? "_frente.png" : "_espalda.png");
    SDL_Texture* textura = cargarTextura(ruta);
    if(!textura){
        return;
    }
    float ancho = 0, alto = 0;
    SDL_GetTextureSize(textura, &ancho, &alto);
    SDL_FRect destino = {x, y, ancho * escala, alto * escala};
    SDL_RenderTexture(m_renderer, textura, nullptr, &destino);
}

void Interfaz::dibujarRectangulo(float x, float y, float ancho, float alto,
                                 SDL_Color color, bool relleno)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_FRect rect = {x, y, ancho, alto};
    if(relleno){
        SDL_RenderFillRect(m_renderer, &rect);
    }else{
        SDL_RenderRect(m_renderer, &rect);
    }
}

// ============================================================================
// 2. PANTALLAS: TU PARTE
//    Versión provisional solo con texto, para que el juego ya se pueda jugar.
//    Sustitúyela por la tuya: tarjetas, sprites, barras de vida...
// ============================================================================

void Interfaz::dibujarSeleccion(const SeleccionPersonaje& seleccion)
{
    dibujarTexto("ELIGE PERSONAJE", 8, 8);

    const std::vector<Personaje>& opciones = seleccion.getOpciones();
    for(int i = 0; i < static_cast<int>(opciones.size()); i++){
        float y = 32.0f + i * 16.0f;
        if(i == seleccion.getIndiceMarcado()){
            dibujarTexto(">", 8, y, {255, 220, 0, 255});
        }
        dibujarTexto(opciones[i].getNombre(), 24, y);
    }

    dibujarTexto("Flechas y Enter", 8, 144, {150, 150, 150, 255});
}

void Interfaz::dibujarCombate(const Combate& combate, int opcionMenu)
{
    const Personaje& jugador = combate.getJugador();
    const Personaje& rival = combate.getRival();

    // Rival arriba, jugador abajo (como en Pokémon)
    dibujarTexto(rival.getNombre() + " " + std::to_string(rival.getPsActuales()) +
                 "/" + std::to_string(rival.getPsMax()), 8, 8);
    dibujarTexto(jugador.getNombre() + " " + std::to_string(jugador.getPsActuales()) +
                 "/" + std::to_string(jugador.getPsMax()), 8, 40);

    if(combate.haTerminado()){
        bool ganaJugador = (combate.getGanador().value() == Lado::Jugador);
        dibujarTexto(ganaJugador ? "HAS GANADO!" : "HAS PERDIDO", 8, 88, {255, 220, 0, 255});
        dibujarTexto("Esc para salir", 8, 104, {150, 150, 150, 255});
        return;
    }

    if(combate.getTurno() == Lado::Rival){
        dibujarTexto("Turno del rival...", 8, 88);
        return;
    }

    // Menú de habilidades del jugador
    const std::vector<Habilidad>& habilidades = jugador.getHabilidades();
    for(int i = 0; i < static_cast<int>(habilidades.size()); i++){
        float y = 88.0f + i * 12.0f;
        if(i == opcionMenu){
            dibujarTexto(">", 8, y, {255, 220, 0, 255});
        }
        dibujarTexto(habilidades[i].getNombre(), 20, y);
    }
}
