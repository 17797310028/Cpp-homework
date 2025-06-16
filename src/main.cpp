#include <iostream>
#include "piece.h"

class King : public Piece {
public:
    King() : Piece("King", 150, 20) {}
};

class Doctor : public Piece {
public:
    Doctor() : Piece("Doctor", 120, 10) {}
};

class Bow : public Piece {
public:
    Bow() : Piece("Bow", 100, 15) {}
};

class Sword : public Piece {
public:
    Sword() : Piece("Sword", 110, 18) {}
};

int main() {
    King king;
    Doctor doctor;
    Bow bow;
    Sword sword;

    std::cout << king.getName() << " HP:" << king.getHP()
              << " ATK:" << king.getAttack() << std::endl;
    std::cout << doctor.getName() << " HP:" << doctor.getHP()
              << " ATK:" << doctor.getAttack() << std::endl;
    std::cout << bow.getName() << " HP:" << bow.getHP()
              << " ATK:" << bow.getAttack() << std::endl;
    std::cout << sword.getName() << " HP:" << sword.getHP()
              << " ATK:" << sword.getAttack() << std::endl;
    return 0;
}
