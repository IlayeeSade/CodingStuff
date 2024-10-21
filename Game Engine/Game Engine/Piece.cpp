#include "Piece.h"

SDL_Renderer* Piece::renderer = nullptr;
Uint16 Piece::board_xsp = 0;
Uint16 Piece::board_ysp = 0;
Uint8 Piece::board_size = 0;

// Constructor and destructor for the piece

Piece::Piece(std::string path, int x, int y, bool isWhite) : cords({x, y}), isWhite(isWhite) {
	rect = new SDL_Rect({board_xsp + x * board_size, board_ysp + y * board_size, board_size, board_size});
    try {
        //std::tuple<SDL_Texture*,SDL_Surface*> tuple = loaders::loadTextureAndSurface(path, renderer);
        //texture = std::get<0>(tuple);
        //surface = std::get<1>(tuple);
		texture = loaders::loadTexture(path, renderer);
		surface = loaders::resizeSurface(loaders::loadSurface(path), 100, 100);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        helpers::quit();
    }
}

Piece::~Piece() {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    delete rect;
}

// Render the piece

void Piece::render() {
    if (rectDirty) {
        delete rect;
        rect = new SDL_Rect({board_xsp + cords.first * board_size, board_ysp + cords.second * board_size, board_size, board_size});
        rectDirty = false;
    }
    SDL_RenderCopy(renderer, texture, NULL, rect);
}

// Getters and setters

SDL_Rect* Piece::getRect() const {
	return rect;
}

SDL_Surface* Piece::getSurface() const {
	return surface;
}

SDL_Texture* Piece::getTexture() const {
	return texture;
}

int Piece::getX() const {
    return cords.first;
}

int Piece::getY() const {
    return cords.second;
}

void Piece::setRect(SDL_Rect* rect) {
	this->rect = rect;
}

void Piece::setSurface(SDL_Surface* surface) {
	this->surface = surface;
}

void Piece::setTexture(SDL_Texture* texture) {
	this->texture = texture;
}

bool Piece::getIsWhite() const {
    return isWhite;
}

void Piece::setCords(int x, int y) {
	cords = {x, y};
	rectDirty = true;
}

std::pair<int, int> Piece::getCords() const {
    return cords;
}

// getValidMoves for each piece - GPT Generated, revised by me

std::vector<std::pair<int, int>> Pawn::getValidMoves(Piece* board[8][8]) const {
    std::vector<std::pair<int, int>> moves;
    int direction = isWhite ? -1 : 1;
    int startRow = isWhite ? 6 : 1;

    int forwardX = cords.first;
    int forwardY = cords.second + direction;

    if (forwardY >= 0 && forwardY < 8 && board[forwardX][forwardY] == nullptr) {
        moves.push_back({ forwardX, forwardY });

        if (cords.second == startRow) {
            int doubleForwardY = cords.second + 2 * direction;
            if (board[forwardX][doubleForwardY] == nullptr) {
                moves.push_back({ forwardX, doubleForwardY });
            }
        }
    }

    for (int dx = -1; dx <= 1; dx += 2) {
        int captureX = cords.first + dx;
        int captureY = cords.second + direction;

        if (captureX >= 0 && captureX < 8 && captureY >= 0 && captureY < 8) {
            Piece* target = board[captureX][captureY];
            if (target && target->getIsWhite() != isWhite) {
                moves.push_back({ captureX, captureY });
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> Rook::getValidMoves(Piece* board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    // Directions: Up, Down, Left, Right
    int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    for (auto& dir : directions) {
        int newX = cords.first;
        int newY = cords.second;

        while (true) {
            newX += dir[0];
            newY += dir[1];

            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
                break;

            Piece* target = board[newX][newY];

            if (target == nullptr) {
                moves.push_back({ newX, newY });
            }
            else {
                if (target->getIsWhite() != isWhite)
                    moves.push_back({ newX, newY });
                break;
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> Knight::getValidMoves(Piece* board[8][8]) const {
    std::vector<std::pair<int, int>> moves;
    int offsets[8][2] = {
        {1, 2}, {2, 1}, {-1, 2}, {-2, 1},
        {-1, -2}, {-2, -1}, {1, -2}, {2, -1}
    };

    for (auto& offset : offsets) {
        int newX = cords.first + offset[0];
        int newY = cords.second + offset[1];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            Piece* target = board[newX][newY];
            if (target == nullptr || target->getIsWhite() != isWhite) {
                moves.push_back({ newX, newY });
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> Bishop::getValidMoves(Piece* board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    // Directions: Diagonals
    int directions[4][2] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };

    for (auto& dir : directions) {
        int newX = cords.first;
        int newY = cords.second;

        while (true) {
            newX += dir[0];
            newY += dir[1];

            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
                break;

            Piece* target = board[newX][newY];

            if (target == nullptr) {
                moves.push_back({ newX, newY });
            }
            else {
                if (target->getIsWhite() != isWhite)
                    moves.push_back({ newX, newY });
                break;
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> Queen::getValidMoves(Piece* board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    // Define all 8 possible directions
    int directions[8][2] = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0},
        {1, 1},
        {1, -1},
        {-1, -1},
        {-1, 1}
    };

    for (auto& dir : directions) {
        int newX = cords.first + dir[0];
        int newY = cords.second + dir[1];

        while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            Piece* target = board[newX][newY];

            if (target == nullptr) {
                moves.emplace_back(newX, newY);
            }
            else {
                if (target->getIsWhite() != isWhite) {
                    moves.emplace_back(newX, newY);
                }
                break;
            }
            newX += dir[0];
            newY += dir[1];
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> King::getValidMoves(Piece* board[8][8]) const {
    std::vector<std::pair<int, int>> moves;

    int offsets[8][2] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };

    for (auto& offset : offsets) {
        int newX = cords.first + offset[0];
        int newY = cords.second + offset[1];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            Piece* target = board[newX][newY];
            if (target == nullptr || target->getIsWhite() != isWhite) {
                // TODO: Check if moving to (newX, newY) puts the king in check
                moves.push_back({ newX, newY });
            }
        }
    }

    // TODO: Implement castling logic if needed

    return moves;
}
