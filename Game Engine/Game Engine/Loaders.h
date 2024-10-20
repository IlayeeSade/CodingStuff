#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <tuple>
#include <iostream>

namespace loaders {
    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
    SDL_Texture* loadTextureFromSurface(const std::string& path, SDL_Renderer* renderer, SDL_Surface* surface);
    SDL_Surface* loadSurface(const std::string& path);
    std::tuple<SDL_Texture*,SDL_Surface*> loadTextureAndSurface(const std::string& path, SDL_Renderer* renderer);
    SDL_Surface* resizeSurface(SDL_Surface* surface, int new_width, int new_height);
}
