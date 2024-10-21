#include "Board.h"

// Constructor

Board::Board(Uint16 size, Uint16 xsp, Uint16 ysp) : board_size(size), board_xsp(xsp), board_ysp(ysp), piece_manager(PieceManager::getInstance()), due_piece(nullptr), turn(true) {
}

// Rendering method

void Board::render_board(SDL_Renderer* renderer, SDL_Color a, SDL_Color b) {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            SDL_Rect rect = { board_xsp + x * board_size, board_ysp + y * board_size, board_size, board_size };
            if ((x + y) % 2 == 0) {
                helpers::SetRenderDrawColor(renderer, a);
                SDL_RenderFillRect(renderer, &rect);
            }
            else {
                helpers::SetRenderDrawColor(renderer, b);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    for (const auto& [a, b] : valid_moves) {
        SDL_Rect rect = { board_xsp + a * board_size, board_ysp + b * board_size, board_size, board_size };
        helpers::SetRenderDrawColor(renderer, { 255, 0, 0, 20 });
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Board::render_pieces() {
    piece_manager->renderPieces();
}

// Mouse methods

void Board::mouseDown(int x, int y) {
    // Check if the click is on a valid move
    int i = x / 100;
    int j = y / 100;
    if (due_piece) {
        for (const auto& [a, b] : valid_moves) {
            if (a == i && b == j) {
                piece_manager->movePiece(due_piece, i, j);
                valid_moves.clear();
                turn = !turn;
                due_piece = nullptr;
                return;
            }
        }
        due_piece = piece_manager->getPiece(i, j);
        if (due_piece) {
            if (!(due_piece->getIsWhite() ^ turn)) {
                valid_moves = piece_manager->mouseDown(due_piece, x, y);
            }
            else {
                valid_moves.clear();
                due_piece = nullptr;
            }
        }
    }
    else {
        due_piece = piece_manager->getPiece(i, j);
        if (due_piece) {
            if (!(due_piece->getIsWhite() ^ turn)) {
                valid_moves = piece_manager->mouseDown(due_piece, x, y);
            }
            else {
                due_piece = nullptr;
            }
		}
    }
}