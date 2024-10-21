#include "PieceManager.h"

PieceManager* PieceManager::instance = nullptr;

// Constructor and Deconstructor

PieceManager::PieceManager() {
    // Initialize board to easily handle empty ptrs
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            board[x][y] = nullptr;
        }
    }

    // Initialize pawns
    for (int i = 0; i < 8; i++) {
        board[i][6] = new Pawn(true, i, 6);
        board[i][1] = new Pawn(false, i, 1);
    }

    // White pieces
    board[0][7] = new Rook(true, 0, 7);
    board[7][7] = new Rook(true, 7, 7);
    board[1][7] = new Knight(true, 1, 7);
    board[6][7] = new Knight(true, 6, 7);
    board[2][7] = new Bishop(true, 2, 7);
    board[5][7] = new Bishop(true, 5, 7);
    board[3][7] = new Queen(true, 3, 7);
    board[4][7] = new King(true, 4, 7);

    // Black pieces
    board[0][0] = new Rook(false, 0, 0);
    board[7][0] = new Rook(false, 7, 0);
    board[1][0] = new Knight(false, 1, 0);
    board[6][0] = new Knight(false, 6, 0);
    board[2][0] = new Bishop(false, 2, 0);
    board[5][0] = new Bishop(false, 5, 0);
    board[3][0] = new Queen(false, 3, 0);
    board[4][0] = new King(false, 4, 0);
}

PieceManager::~PieceManager() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            delete board[x][y];
        }
    }
}

// Get instance

PieceManager* PieceManager::getInstance() {
    if (!instance) {
        instance = new PieceManager();
    }
    return instance;
}

// Rendering methods

void PieceManager::renderPieces() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board[x][y]) {
                board[x][y]->render();
            }
        }
    }
}

// Mouse methods

std::vector<std::pair<int, int>> PieceManager::mouseDown(const Piece* piece, int x, int y) {
    SDL_Point clickPoint = { x, y };
    if (piece) {
        // Check if the click is within the piece
        SDL_Rect* rect = piece->getRect();
        if (SDL_PointInRect(&clickPoint, rect)) {
            // Relative position logic
            int relativeX = x - rect->x;
            int relativeY = y - rect->y;
            SDL_Surface* surface = piece->getSurface();
            int w = surface->w;
            int h = surface->h;
            // Ensure relative positions are in the cell
            if (relativeX >= 0 && relativeX < w && relativeY >= 0 && relativeY < h) {
                Uint32* pixels = (Uint32*)surface->pixels;
                int pixelIndex = relativeY * surface->w + relativeX;
                Uint32 pixel = pixels[pixelIndex];

                // Check alpha
                Uint8 alpha = (pixel & surface->format->Amask) >> surface->format->Ashift;

                if (alpha > ALPHA_THRESHOLD) {
                    return piece->getValidMoves(board);
                }
            }
        }
    }
    return std::vector<std::pair<int, int>>();
}

// Getters & Setters 

Piece* PieceManager::getPiece(int x, int y) const{
    return board[x][y];
}

void PieceManager::movePiece(Piece* piece, int x, int y) {
    std::pair<int, int> cords = piece->getCords();
	board[cords.first][cords.second] = nullptr;
	piece->setCords(x, y);
	board[x][y] = piece;
}

