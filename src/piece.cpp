#include "piece.h"

Piece::Piece(const std::string& name, int hp, int attack)
    : name_(name), hp_(hp), attack_(attack) {}

Piece::~Piece() = default;

const std::string& Piece::getName() const { return name_; }
int Piece::getHP() const { return hp_; }
int Piece::getAttack() const { return attack_; }
void Piece::setHP(int hp) { hp_ = hp; }
void Piece::setAttack(int attack) { attack_ = attack; }
