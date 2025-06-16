#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>
#include "piece.h"

class Map {
public:
    Map(int width, int height);

    void placePiece(std::unique_ptr<Piece> piece, int x, int y);
    Piece* getPiece(int x, int y) const;

    int getWidth() const;
    int getHeight() const;

private:
    int width_;
    int height_;
    std::vector<std::vector<std::unique_ptr<Piece>>> grid_;
};

#endif // MAP_H
