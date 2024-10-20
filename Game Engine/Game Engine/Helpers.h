#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include "Piece.h"

namespace helpers {
    Uint8 init(SDL_Renderer*& renderer, SDL_Window*& window, Uint16 SCREEN_WIDTH, Uint16 SCREEN_HEIGHT);
    void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);
    void quit();
}