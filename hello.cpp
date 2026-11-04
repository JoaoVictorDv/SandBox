#include <SDL3/SDL.h>
#include <string>

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
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

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
