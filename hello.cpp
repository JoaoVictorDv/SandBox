#include <SDL3/SDL.h>
#include <string>
#include <vector>

struct pixel {
    float x;
    float y;
    float velocidadeY;
    bool corTerra;
};

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Minha janela SDL3", 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (window == nullptr || renderer == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool aberta = true;
    SDL_Event evento;
    float mouseX = 400;
    float mouseY = 300;
    std::vector<pixel> cliques;

    constexpr float larguraJanela = 800;
    constexpr float alturaJanela = 600;
    constexpr float lado = 8;
    constexpr float gravidade = 800.0f;
    Uint64 ultimoTempo = SDL_GetTicks();

    while (aberta) {
        Uint64 agora = SDL_GetTicks();
        float deltaTempo = (agora - ultimoTempo) / 1000.0f;
        ultimoTempo = agora;

        for (pixel& clique : cliques) {
            clique.velocidadeY += gravidade * deltaTempo;
            float proximoY = clique.y + clique.velocidadeY * deltaTempo;
            float limiteY = alturaJanela - lado / 2;

            for (const pixel& outro : cliques) {
                if (&clique == &outro) {
                    continue;
                }

                bool sobrepoeNaHorizontal =
                    clique.x + lado / 2 > outro.x - lado / 2 &&
                    clique.x - lado / 2 < outro.x + lado / 2;
                bool estavaAcima = clique.y <= outro.y;
                bool atingiuOTopoDoOutro = proximoY + lado / 2 >= outro.y - lado / 2;

                if (sobrepoeNaHorizontal && estavaAcima && atingiuOTopoDoOutro) {
                    float topoParaParar = outro.y - lado;
                    if (topoParaParar < limiteY) {
                        limiteY = topoParaParar;
                    }
                }
            }

            if (proximoY > limiteY) {
                clique.y = limiteY;
                clique.velocidadeY = 0;
            } else {
                clique.y = proximoY;
            }
        }

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                aberta = false;
            }
            else if (evento.type == SDL_EVENT_MOUSE_MOTION) {
                mouseX = evento.motion.x;
                mouseY = evento.motion.y;

                SDL_SetWindowTitle(
                    window,
                    ("Mouse: " + std::to_string(mouseX) + ", " + std::to_string(mouseY)).c_str()
                );
            }
            else if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (evento.button.button == SDL_BUTTON_LEFT) {
                    cliques.push_back({evento.button.x, evento.button.y, 0, false});
                }
                else if (evento.button.button == SDL_BUTTON_RIGHT) {
                    for (pixel& clique : cliques) {
                        bool clicouNoQuadrado =
                            evento.button.x >= clique.x - lado / 2 &&
                            evento.button.x <= clique.x + lado / 2 &&
                            evento.button.y >= clique.y - lado / 2 &&
                            evento.button.y <= clique.y + lado / 2;

                        if (clicouNoQuadrado) {
                            clique.corTerra = true;
                            break;
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (const pixel& clique : cliques) {
            if (clique.corTerra) {
                SDL_SetRenderDrawColor(renderer, 121, 85, 61, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }

            SDL_FRect quadrado = {
                clique.x - lado / 2,
                clique.y - lado / 2,
                lado,
                lado
            };

            SDL_RenderFillRect(renderer, &quadrado);
        }

        SDL_FRect cursor = {mouseX - 5, mouseY - 5, 10, 10};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &cursor);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
