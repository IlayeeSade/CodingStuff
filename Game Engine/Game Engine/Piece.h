#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Exceptions.h"
#include "Loaders.h"
#include "Helpers.h"
#include <vector>
#include <utility>

class Piece {
    SDL_Rect* rect = nullptr;
    bool rectDirty = true;

    SDL_Texture* texture;
    SDL_Surface* surface;
protected:
    std::pair<int, int> cords;
    bool isWhite;
public:
    static SDL_Renderer* renderer;
    static Uint8 board_size;
    static Uint16 board_xsp;
    static Uint16 board_ysp;

    Piece(std::string path, int x, int y, bool isWhite);
    ~Piece();

    // Render the piece
    virtual void render();

    // Get valid moves
    virtual std::vector<std::pair<int, int>> getValidMoves(Piece* board[8][8]) = 0;

    // Getters and Setters
    SDL_Rect* getRect();
    SDL_Surface* getSurface();
    SDL_Texture* getTexture();

    void setRect(SDL_Rect* rect);
    void setSurface(SDL_Surface* surface);
    void setTexture(SDL_Texture* texture);
    int getX();
    int getY();


    // Getters & Setters
    bool getIsWhite();
    std::pair<int, int> getCords();
};

// ======================
// Base Virtual Class for All Pieces
// ======================
class King : public virtual Piece {
public:
    King(bool isWhite, int x, int y) : Piece(std::string(R"(C:\Users\elais\source\repos\Game Engine\Game Engine\Assets\Chess_pieces\king)") + (isWhite ? ".png" : "1.png"), x, y, isWhite) {
    }
    std::vector<std::pair<int, int>> getValidMoves(Piece* board[8][8]);
};

class Queen : public virtual Piece {
public:
    Queen(bool isWhite, int x, int y) : Piece(std::string(R"(C:\Users\elais\source\repos\Game Engine\Game Engine\Assets\Chess_pieces\queen)") + (isWhite ? ".png" : "1.png"), x, y, isWhite) {
    }
    std::vector<std::pair<int, int>> getValidMoves(Piece* board[8][8]);
};

class Rook : public virtual Piece {
public:
    Rook(bool isWhite, int x, int y) : Piece(std::string(R"(C:\Users\elais\source\repos\Game Engine\Game Engine\Assets\Chess_pieces\rook)") + (isWhite ? ".png" : "1.png"), x, y, isWhite) {
    }
    std::vector<std::pair<int, int>> getValidMoves(Piece* board[8][8]);
};

class Bishop : public virtual Piece {
public:
    Bishop(bool isWhite, int x, int y) : Piece(std::string(R"(C:\Users\elais\source\repos\Game Engine\Game Engine\Assets\Chess_pieces\bishop)") + (isWhite ? ".png" : "1.png"), x, y, isWhite) {
    }
    std::vector<std::pair<int, int>> getValidMoves(Piece* board[8][8]);
};

class Knight : public virtual Piece {
public:
    Knight(bool isWhite, int x, int y) : Piece(std::string(R"(C:\Users\elais\source\repos\Game Engine\Game Engine\Assets\Chess_pieces\knight)") + (isWhite ? ".png" : "1.png"), x, y, isWhite) {
    }
    std::vector<std::pair<int, int>> getValidMoves(Piece* board[8][8]);
};

class Pawn : public virtual Piece {
public:
    Pawn(bool isWhite, int x, int y) : Piece(std::string(R"(C:\Users\elais\source\repos\Game Engine\Game Engine\Assets\Chess_pieces\pawn)") + (isWhite ? ".png" : "1.png"), x, y, isWhite) {
    }
    std::vector<std::pair<int, int>> getValidMoves(Piece* board[8][8]);
};
