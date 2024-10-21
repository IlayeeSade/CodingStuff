#pragma once
#include <iostream>
#include <SDL.h>
#include "Piece.h"
#define BOARD_LENGTH 8
#define ALPHA_THRESHOLD 0

// Singlton class for now

class PieceManager {
    static PieceManager* instance;
    Piece* board[BOARD_LENGTH][BOARD_LENGTH]; // Board array of pieces, considered using a vector for exisiting pieces but this is simpler and does not affect performance.

    PieceManager();
    ~PieceManager();
    PieceManager(const PieceManager&) = delete;
    PieceManager& operator=(const PieceManager&) = delete;
public:
    static PieceManager* getInstance();

    void renderPieces();

    std::vector<std::pair<int, int>> mouseDown(const Piece* piece, int x, int y);

    Piece* getPiece(int x, int y) const;
    void movePiece(Piece* piece, int x, int y);
};
