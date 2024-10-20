#pragma once
#include <string>

class UnableToLoadImage : public std::exception {
	std::string message;
public:
	UnableToLoadImage(const std::string& path) {
		message = "Unable to load image " + path + "! SDL_image Error: " + IMG_GetError();
	}
	const char* what() const throw() {
		return message.c_str();
	}
};

class UnableToCreateTextureFromSurface : public std::exception {
	std::string message;
public:
	UnableToCreateTextureFromSurface(const std::string& path) {
		message = "Unable to create texture from " + path + "! SDL Error: " + SDL_GetError();
	}
	const char* what() const throw() {
		return message.c_str();
	}
};

class UnableToOptimizeSurface : public std::exception {
	std::string message;
public:
	UnableToOptimizeSurface(const std::string& path) {
		message = "Unable to optimize surface from " + path + "! SDL Error: " + SDL_GetError();
	}
	const char* what() const throw() {
		return message.c_str();
	}
};

class UnableToCreateRGBSurface : public std::exception {
	std::string message;
public:
	UnableToCreateRGBSurface() {
		message = "Unable to resize surface, SDL_CreateRGBSurface failed: " + std::string(SDL_GetError());
	}
	const char* what() const throw() {
		return message.c_str();
	}
};

class UnableToResizeSurface : public std::exception {
	std::string message;
public:
	UnableToResizeSurface() {
		message = "Unable to resize surface, SDL_BlitScaled failed: " + std::string(SDL_GetError());
	}
	const char* what() const throw() {
		return message.c_str();
	}
};