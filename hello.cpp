#include <SDL3/SDL.h>
#include <string>
#include <vector>

struct pixel {
    float x;
    float y;
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

    while (aberta) {
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
                    cliques.push_back({evento.button.x, evento.button.y});
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        constexpr float lado = 24;

        for (const pixel& clique : cliques) {
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
