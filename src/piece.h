#pragma once
#include <string>

class Piece {
public:
    Piece(const std::string& name, int hp, int attack);
    virtual ~Piece();

    const std::string& getName() const;
    int getHP() const;
    int getAttack() const;

    void setHP(int hp);
    void setAttack(int attack);

private:
    std::string name_;
    int hp_;
    int attack_;
};
