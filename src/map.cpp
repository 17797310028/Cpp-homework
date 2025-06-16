#include "map.h"

Map::Map(int width, int height) : width_(width), height_(height), grid_(height) {
    for (auto& row : grid_) {
        row.resize(width);
    }
}

void Map::placePiece(std::unique_ptr<Piece> piece, int x, int y) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        grid_[y][x] = std::move(piece);
    }
}

Piece* Map::getPiece(int x, int y) const {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        return grid_[y][x].get();
    }
    return nullptr;
}

int Map::getWidth() const { return width_; }
int Map::getHeight() const { return height_; }
