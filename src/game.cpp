#include "game.h"
#include "king.h"
#include "doctor.h"
#include "bow.h"
#include "sword.h"
#include <iostream>
#include <memory>

Game::Game(int width, int height)
    : map_(width, height), turn_(0) {
    map_.placePiece(std::make_unique<King>(), 0, 0);
    map_.placePiece(std::make_unique<Doctor>(), 1, 0);
    map_.placePiece(std::make_unique<Bow>(), 0, 1);
    map_.placePiece(std::make_unique<Sword>(), 1, 1);
}

void Game::run(int turns) {
    for (int i = 0; i < turns; ++i) {
        ++turn_;
        std::cout << "Turn: " << turn_ << std::endl;
        // TODO: Add actions for each turn
    }
}
