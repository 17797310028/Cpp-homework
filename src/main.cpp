#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Position {
    int x;
    int y;
};

class Piece {
public:
    Piece(const std::string& name, int hp, int attack, int x = 0, int y = 0)
        : name_(name), hp_(hp), attack_(attack), pos_{x, y} {}
    virtual ~Piece() = default;

    const std::string& getName() const { return name_; }
    int getHP() const { return hp_; }
    int getAttack() const { return attack_; }
    int getX() const { return pos_.x; }
    int getY() const { return pos_.y; }

    void setHP(int hp) { hp_ = hp; }
    void setAttack(int attack) { attack_ = attack; }
    void setPosition(int x, int y) { pos_.x = x; pos_.y = y; }

private:
    std::string name_;
    int hp_;
    int attack_;
    Position pos_;
};

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

class Map {
public:
    void addPiece(std::unique_ptr<Piece> p) {
        pieces_.push_back(std::move(p));
    }

    const std::vector<std::unique_ptr<Piece>>& getPieces() const { return pieces_; }

    void printPieces() const {
        for (const auto& p : pieces_) {
            std::cout << p->getName() << " (" << p->getX() << "," << p->getY() << ") "
                      << "HP:" << p->getHP() << " ATK:" << p->getAttack() << std::endl;
        }
    }

private:
    std::vector<std::unique_ptr<Piece>> pieces_;
};

int main() {
    Map map;

    auto king = std::make_unique<King>();
    king->setPosition(0, 0);
    map.addPiece(std::move(king));

    auto doctor = std::make_unique<Doctor>();
    doctor->setPosition(1, 0);
    map.addPiece(std::move(doctor));

    auto bow = std::make_unique<Bow>();
    bow->setPosition(0, 1);
    map.addPiece(std::move(bow));

    auto sword = std::make_unique<Sword>();
    sword->setPosition(1, 1);
    map.addPiece(std::move(sword));

    map.printPieces();
    return 0;
}
