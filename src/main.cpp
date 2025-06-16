#include "piece.h"
#include <iostream>

int main() {
    Piece p("Knight", 100, 15);
    std::cout << "Piece: " << p.getName() << " HP: " << p.getHP() << " Attack: " << p.getAttack() << std::endl;
    return 0;
}
