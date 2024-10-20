#include "Helpers.h"

namespace helpers {
    SDL_Renderer* gRenderer = nullptr;
    SDL_Window* gWindow = nullptr;

    Uint8 init(SDL_Renderer*& renderer, SDL_Window*& window, Uint16 SCREEN_WIDTH, Uint16 SCREEN_HEIGHT) {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }

        // Initialize SDL_image
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        // Initialize Window and Renderer
        window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
        gWindow = window;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        gRenderer = renderer;

        return 0;
    }

    void quit() {
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        IMG_Quit();
        SDL_Quit();
    }

    void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
}