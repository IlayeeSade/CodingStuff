#include "Loaders.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Exceptions.h"

namespace loaders {
    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
        SDL_Texture* newTexture = nullptr;
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == nullptr) {
            throw UnableToLoadImage(path);
        }
        else {
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (newTexture == nullptr) {
                SDL_FreeSurface(loadedSurface);
                throw UnableToCreateTextureFromSurface(path);
            }
            SDL_FreeSurface(loadedSurface);
        }
        return newTexture;
    }

    SDL_Texture* loadTextureFromSurface(const std::string& path, SDL_Renderer* renderer, SDL_Surface* surface) {
        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if (newTexture == nullptr) {
            SDL_FreeSurface(surface);
            throw UnableToCreateTextureFromSurface(path);
        }
        SDL_FreeSurface(surface);
        return newTexture;
    }

    SDL_Surface* loadSurface(const std::string& path) {
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == nullptr) {
            throw UnableToLoadImage(path);
        }

        // Convert surface to RGBA32 format for consistent pixel access
        SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA32, 0);
        if (optimizedSurface == nullptr) {
            SDL_FreeSurface(loadedSurface);
            throw UnableToOptimizeSurface(path);
        }
        SDL_FreeSurface(loadedSurface);
        return optimizedSurface;
	}

    std::tuple<SDL_Texture*, SDL_Surface*> loadTextureAndSurface(const std::string& path, SDL_Renderer* renderer) {
        SDL_Surface* surface = loadSurface(path);
        SDL_Texture* newTexture = loadTextureFromSurface(path, renderer, surface);
        return std::make_tuple(newTexture, surface);
    }

    SDL_Surface* resizeSurface(SDL_Surface* surface, int new_width, int new_height) {
        SDL_Surface* resized_surface = SDL_CreateRGBSurface(0, new_width, new_height, surface->format->BitsPerPixel,
            surface->format->Rmask, surface->format->Gmask,
            surface->format->Bmask, surface->format->Amask);
        if (resized_surface == nullptr) {
            throw UnableToCreateRGBSurface();
        }

        // Scale the original surface onto the new surface
        SDL_Rect src_rect = { 0, 0, surface->w, surface->h };
        SDL_Rect dst_rect = { 0, 0, new_width, new_height };

        if (SDL_BlitScaled(surface, &src_rect, resized_surface, &dst_rect) < 0) {
            SDL_FreeSurface(resized_surface);
            throw UnableToResizeSurface();
        }

        return resized_surface;
    }
}
