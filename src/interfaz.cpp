#include "interfaz.h"
#include <SDL3_image/SDL_image.h>
#include <algorithm>


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

    // Ventana x4 (960x640)
    m_ventana = SDL_CreateWindow("JRPG", kAncho * 4, kAlto * 4, 0);
    m_renderer = SDL_CreateRenderer(m_ventana, nullptr);
    if(!m_ventana || !m_renderer){
        SDL_Log("Error creando la ventana: %s", SDL_GetError());
        return;
    }

    // Escalado sin difuminar (pixel art)
    SDL_SetRenderLogicalPresentation(m_renderer, kAncho, kAlto,
                                     SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    SDL_SetDefaultTextureScaleMode(m_renderer, SDL_SCALEMODE_PIXELART);

    // Press Start 2P a 8 px
    m_fuente = TTF_OpenFont("assets/fuentes/PressStart2P-Regular.ttf", 8);
    if(!m_fuente){
        SDL_Log("No se pudo cargar la fuente: %s", SDL_GetError());
        return;
    }

    m_abierta = true;
}

Interfaz::~Interfaz()
{
    // Orden inverso a la creación
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
        // Se ignora la repetición automática
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
   SDL_SetRenderDrawColor(m_renderer, 20, 20, 40, 255);
    SDL_RenderClear(m_renderer);

    if(SDL_GetTicks() < m_finTemblor){
        m_offX = (SDL_rand(3) - 1) * m_intensidadTemblor;
        m_offY = (SDL_rand(3) - 1) * m_intensidadTemblor;
    }else{
        m_offX = 0;
        m_offY = 0;
    }
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
    SDL_FRect destino = {x + m_offX, y + m_offY, ancho, alto};
    SDL_RenderTexture(m_renderer, textura, nullptr, &destino);

    SDL_DestroyTexture(textura);
}

void Interfaz::dibujarTextoAjustado(const std::string& texto, float x, float y, int anchoMaximo,
                                    SDL_Color color)
{
    if(texto.empty() || !m_fuente){
        return;
    }
    SDL_Surface* superficie = TTF_RenderText_Solid_Wrapped(m_fuente, texto.c_str(), 0, color, anchoMaximo);
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
    SDL_FRect destino = {x + m_offX, y + m_offY, ancho, alto};
    SDL_RenderTexture(m_renderer, textura, nullptr, &destino);
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
    auto encontrada = m_texturas.find(ruta);
    if(encontrada != m_texturas.end()){
        return encontrada->second;
    }

    SDL_Texture* textura = IMG_LoadTexture(m_renderer, ruta.c_str());
    if(!textura){
        SDL_Log("No se pudo cargar %s: %s", ruta.c_str(), SDL_GetError());
    }
    m_texturas[ruta] = textura;   // también nullptr, para no reintentarlo
    return textura;
}

void Interfaz::dibujarSprite(const std::string& nombreSprite, bool deFrente,
                             float x, float y, float escala, SDL_Color tinte)
{
    std::string ruta = "assets/sprites/" + nombreSprite + (deFrente ? "_frente.png" : "_espalda.png");
    SDL_Texture* textura = cargarTextura(ruta);
    if(!textura){
        return;
    }
    float ancho = 0, alto = 0;
    SDL_GetTextureSize(textura, &ancho, &alto);
    SDL_FRect destino = {x + m_offX, y + m_offY, ancho * escala, alto * escala};

    // La textura está en la caché: se restaura el color después de usarla
    SDL_SetTextureColorMod(textura, tinte.r, tinte.g, tinte.b);
    SDL_RenderTexture(m_renderer, textura, nullptr, &destino);
    SDL_SetTextureColorMod(textura, 255, 255, 255);
}

void Interfaz::dibujarRectangulo(float x, float y, float ancho, float alto,
                                 SDL_Color color, bool relleno)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_FRect rect = {x + m_offX, y + m_offY, ancho, alto};
    if(relleno){
        SDL_RenderFillRect(m_renderer, &rect);
    }else{
        SDL_RenderRect(m_renderer, &rect);
    }
}



void Interfaz::dibujarSeleccion(const SeleccionPersonaje& seleccion)
{
    
    std::string titulo="ELIGE PERSONAJE";
    dibujarTexto(titulo, (kAncho - anchoTexto(titulo)) / 2.0f, 16);


    const std::vector<Personaje>& opciones = seleccion.getOpciones();
    float ancho = 56;      // ancho de cada tarjeta
    float y = 50;          // altura de las tarjetas (igual para todas)

    for(int i = 0; i < static_cast<int>(opciones.size()); i++){
        const Personaje& p = opciones[i];
        float x = 2.0f + i * (ancho + 4);          // 2, 62, 122, 182
        bool marcada = (i == seleccion.getIndiceMarcado());

        // Dorado si está marcada, azul si no
        SDL_Color colorFondo = marcada ? SDL_Color{255, 200, 0, 255}   : SDL_Color{40, 40, 80, 255};
        SDL_Color colorTexto = marcada ? SDL_Color{0, 0, 0, 255}       : SDL_Color{255, 255, 255, 255};
     // Rectángulo del nombre
        dibujarRectangulo(x, y + 50, ancho, -50, colorFondo);
        // Sprite encima
        dibujarSprite(p.getNombreSprite(), true, x + (ancho - 32) / 2, y, 0.5f);

   

        // Nombre centrado
        float anchoNombre = static_cast<float>(anchoTexto(p.getNombre()));
        dibujarTexto(p.getNombre(), x + (ancho - anchoNombre) / 2, y + 38, colorTexto);
    }

    // Ayuda abajo, centrada
    std::string ayuda = "Flechas y Enter";
    dibujarTexto(ayuda, (kAncho - anchoTexto(ayuda)) / 2.0f, 140, {150, 150, 150, 255});
}

void Interfaz::dibujarEstado(const Personaje& personaje, float x, float y)
{
    SDL_Color naranja = {255, 160, 60, 255};
    SDL_Color celeste = {120, 200, 255, 255};
    SDL_Color morado  = {200, 140, 255, 255};

    // Correa: estado y multiplicador max(1, contador)
    if(personaje.getTipo() == Tipo::Correa){
        if(personaje.tieneCorrea()){
            int multiplicador = std::max(1, personaje.turnosConCorrea());
            dibujarTexto("Correa x" + std::to_string(multiplicador), x, y, naranja);
        }else{
            int multiplicador = std::max(1, personaje.turnosLibre());
            dibujarTexto("Libre x" + std::to_string(multiplicador), x, y, celeste);
        }
        y += 10;
    }

    if(personaje.estaAturdido()){
        dibujarTexto("Aturdido", x, y, {200, 200, 200, 255});  
        y += 10;
    }

    if(personaje.estaCanalizando()){
        dibujarTexto("Canaliza", x, y, morado);
    }
}

void Interfaz::dibujarCombate(const Combate& combate, int opcionMenu)
{
    procesarEvento(combate);

    const Personaje& jugador = combate.getJugador();
    const Personaje& rival = combate.getRival();
    SDL_Color amarillo = {255, 220, 0, 255};
    SDL_Color gris = {150, 150, 150, 255};

    Uint64 ahora = SDL_GetTicks();

    // Dibuja el sprite de un bando aplicando sus efectos.
    // haciaX/haciaY: dirección hacia el otro personaje (para la embestida)
    auto dibujarConEfectos = [&](Lado lado, const Personaje& p, bool deFrente,
                                 float x, float y, float haciaX, float haciaY){
        const EfectosLado& ef = efectos(lado);

        // Parpadeo: oculto en frames alternos de 80 ms
        if(ahora < ef.finParpadeo && (ahora / 80) % 2 == 0){
            return;
        }
        if(ahora < ef.finEmbestida){
            x += haciaX * 6;
            y += haciaY * 6;
        }
        SDL_Color tinte = (ahora < ef.finTinte) ? ef.tinte : SDL_Color{255, 255, 255, 255};
        dibujarSprite(p.getNombreSprite(), deFrente, x, y, 0.5f, tinte);
    };

    // Rival: arriba a la izquierda, de frente
    dibujarTexto(rival.getNombre() + " " + std::to_string(rival.getPsActuales()) +
                 "/" + std::to_string(rival.getPsMax()), 8, 4);
    dibujarConEfectos(Lado::Rival, rival, true, 8, 14, 1, 1);           // embiste hacia abajo a la derecha
    dibujarEstado(rival, 44, 16);

    // Jugador: a la derecha, de espaldas
    dibujarTexto(jugador.getNombre() + " " + std::to_string(jugador.getPsActuales()) +
                 "/" + std::to_string(jugador.getPsMax()), 128, 40);
    dibujarConEfectos(Lado::Jugador, jugador, false, 128, 50, -1, -1);  // embiste hacia arriba a la izquierda
    dibujarEstado(jugador, 164, 52);

    // Separador y mensaje
    dibujarRectangulo(0, 86, kAncho, 1, gris);
    dibujarTextoAjustado(combate.getUltimoMensaje(), 8, 90, kAncho - 16);

    // Resultado, turno del rival o menú
    if(combate.haTerminado()){
        bool ganaJugador = (combate.getGanador().value() == Lado::Jugador);
        dibujarTexto(ganaJugador ? "HAS GANADO!" : "HAS PERDIDO", 8, 120, amarillo);
        dibujarTexto("Esc para salir", 8, 136, gris);
        return;
    }

    if(combate.getTurno() == Lado::Rival){
        dibujarTexto("Turno de " + rival.getNombre() + "...", 8, 120, gris);
        return;
    }

    // Menú de habilidades del jugador
    const std::vector<Habilidad>& habilidades = jugador.getHabilidades();
    for(int i = 0; i < static_cast<int>(habilidades.size()); i++){
        float y = 114.0f + i * 12.0f;
        if(i == opcionMenu){
            dibujarTexto(">", 8, y, amarillo);
        }
        dibujarTexto(habilidades[i].getNombre(), 20, y);
    }
}

void Interfaz::procesarEvento(const Combate& combate)
{
    const Combate::Evento& e = combate.getUltimoEvento();
    if(e.numero == m_ultimoEventoVisto){
        return;                         // nada nuevo
    }
    m_ultimoEventoVisto = e.numero;

    Uint64 ahora = SDL_GetTicks();
    Lado atacante = (e.objetivo == Lado::Jugador) ? Lado::Rival : Lado::Jugador;
    EfectosLado& objetivo = efectos(e.objetivo);

    switch(e.efecto){
        case Efecto::Dano:
            m_intensidadTemblor = (e.efectividad > 1.0f) ? 3.0f : 1.0f;
            m_finTemblor = ahora + 300;
            efectos(atacante).finEmbestida = ahora + 150;
            objetivo.finParpadeo = ahora + 500;
            objetivo.finTinte = ahora + 500;
            objetivo.tinte = {255, 90, 90, 255};          // rojo
            break;
        case Efecto::Cura:
            objetivo.finTinte = ahora + 500;
            objetivo.tinte = {120, 255, 120, 255};        // verde
            break;
        case Efecto::Stun:
            efectos(atacante).finEmbestida = ahora + 150;
            objetivo.finTinte = ahora + 700;
            objetivo.tinte = {255, 230, 80, 255};         // amarillo
            break;
        case Efecto::Canalizar:
            objetivo.finTinte = ahora + 700;
            objetivo.tinte = {150, 150, 255, 255};        // azul
            break;
    }
}
