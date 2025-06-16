#include <iostream>
#include <string>
#include <vector>
#include <cstddef>

using namespace std;

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

class Game;  // forward declaration

class Map {
    friend class Game; // 让 Game 直接访问棋盘
public:
    ~Map() {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            delete pieces_[i];
        }
    }

    void addPiece(Piece* p) {
        pieces_.push_back(p);
    }

    const std::vector<Piece*>& getPieces() const { return pieces_; }

    void printPieces() const {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            Piece* p = pieces_[i];
            cout << p->getName() << " (" << p->getX() << "," << p->getY() << ") "
                 << "HP:" << p->getHP() << " ATK:" << p->getAttack() << endl;
        }
    }

private:
    std::vector<Piece*> pieces_;
};

class Game {
public:
    void init() {
        Piece* king = new King();
        king->setPosition(0, 0);
        map_.addPiece(king);

        Piece* doctor = new Doctor();
        doctor->setPosition(1, 0);
        map_.addPiece(doctor);

        Piece* bow = new Bow();
        bow->setPosition(0, 1);
        map_.addPiece(bow);

        Piece* sword = new Sword();
        sword->setPosition(1, 1);
        map_.addPiece(sword);
    }

    void run(int turns) {
        for (int t = 0; t < turns; ++t) {
            cout << "---- 回合 " << (t + 1) << " ----" << endl;
            const vector<Piece*>& pieces = map_.getPieces();
            for (size_t i = 0; i < pieces.size(); ++i) {
                pieces[i]->setHP(pieces[i]->getHP() - 1); // 简单操作
            }
        }
        cout << "最终状态:" << endl;
        map_.printPieces();
    }

private:
    Map map_;
};

int main() {
    Game game;
    game.init();
    game.run(3);
    return 0;
}
