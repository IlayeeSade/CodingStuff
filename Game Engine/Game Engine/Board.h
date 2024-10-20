#pragma once
#include <SDL.h>
#include "Helpers.h"
#include "PieceManager.h"

class Board {
    Uint16 board_size;
    Uint16 board_xsp;
    Uint16 board_ysp;

    PieceManager* piece_manager;
    std::vector<std::pair<int, int>> valid_moves;
public:
    Board(Uint16 size, Uint16 xsp, Uint16 ysp);

    void render_board(SDL_Renderer* renderer, SDL_Color a, SDL_Color b);
    void render_pieces();

    void mouseDown(int x, int y);
};