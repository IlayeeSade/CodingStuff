#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <iostream>
#include <tuple>
#include "Helpers.h"
#include "PieceManager.h"
#include "Board.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800
#define BOARD_SIZE 100
#define BOARD_START_X 0
#define BOARD_START_Y 0

int responsive_delay(Uint32 miliseconds, SDL_Renderer* renderer, SDL_Window* window) {
    Uint32 startTime = SDL_GetTicks();
    bool isWaiting = true;

    while (isWaiting) {
        // Process events to keep the window responsive
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }

        if (SDL_GetTicks() - startTime > miliseconds) {
            isWaiting = false;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {

    // ===============================================
    // Initializations
    // ===============================================

    SDL_Renderer* renderer;
    SDL_Window* window;
    if (helpers::init(renderer, window, SCREEN_WIDTH, SCREEN_HEIGHT)){
        return 1;
    }

    // Set static vars for pieces
    Piece::renderer = renderer;
    Piece::board_xsp = BOARD_START_X;
    Piece::board_ysp = BOARD_START_Y;
    Piece::board_size = BOARD_SIZE;

    // ===============================================
    // Application
    // ===============================================

    // Event loop to keep the window open
    bool isRunning = true;
    SDL_Event event;
    Board b(BOARD_SIZE, BOARD_START_X, BOARD_START_Y);

    while (isRunning) {

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set the draw color to white for clearing
        SDL_RenderClear(renderer);

        b.render_board(renderer, {255,204,114,255}, {70,47,8,255});
        b.render_pieces();


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (event.button.button == SDL_BUTTON_LEFT) {
                    b.mouseDown(x, y);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    IMG_Quit();
    SDL_Quit();

    return 0;
}
