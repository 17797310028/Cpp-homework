#ifndef PIECE_H
#define PIECE_H

#include <string>

class Piece {
public:
    Piece(const std::string& name, int hp, int attack);
    virtual ~Piece() = default;

    const std::string& getName() const;
    int getHP() const;
    int getAttack() const;

    void setHP(int hp);
    void setAttack(int attack);

protected:
    std::string name_;
    int hp_;
    int attack_;
};

#endif // PIECE_H
