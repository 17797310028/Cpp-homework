#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
#include <cstdlib>

using namespace std;

struct Position {
    int x;
    int y;
};

class Player; // forward declaration

class Piece {
    friend class Player; // 允许 Player 修改名称等私有成员
public:
    Piece(const std::string& name, int hp, int attack, int cost, int vision, int x = 0, int y = 0)
        : name_(name), hp_(hp), attack_(attack), cost_(cost), vision_(vision), pos_{x, y} {}
    virtual ~Piece() = default;

    const std::string& getName() const { return name_; }
    int getHP() const { return hp_; }
    int getAttack() const { return attack_; }
    int getX() const { return pos_.x; }
    int getY() const { return pos_.y; }
    int getCost() const { return cost_; }
    int getVision() const { return vision_; }

    void setHP(int hp) { hp_ = hp; }
    void setAttack(int attack) { attack_ = attack; }
    void setPosition(int x, int y) { pos_.x = x; pos_.y = y; }

    void setName(const string& n) { name_ = n; }

private:
    std::string name_;
    int hp_;
    int attack_;
    int cost_;
    int vision_;
    Position pos_;
};

class King : public Piece {
public:
    King() : Piece("King", 150, 20, 5, 2) {}
};

class Doctor : public Piece {
public:
    Doctor() : Piece("Doctor", 120, 10, 4, 2) {}
};

class Bow : public Piece {
public:
    Bow() : Piece("Bow", 100, 15, 3, 3) {}
};

class Sword : public Piece {
public:
    Sword() : Piece("Sword", 110, 18, 4, 1) {}
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

class Player {
    friend class Game;
public:
    Player(const string& name, int gold) : name_(name), gold_(gold) {}

    bool buyPiece(Piece* p, Map& map, int x, int y) {
        int cost = p->getCost();
        if (gold_ < cost) {
            cout << name_ << " 金币不足，无法购买 " << p->getName() << endl;
            delete p;
            return false;
        }
        gold_ -= cost;
        string newName = name_ + "_" + p->getName();
        p->setName(newName);
        p->setPosition(x, y);
        map.addPiece(p);
        pieces_.push_back(p);
        cout << name_ << " 成功购买 " << p->getName() << endl;
        return true;
    }

    void showVisibleEnemies(const Player& enemy) const {
        cout << name_ << " 看到的敌军:" << endl;
        for (size_t i = 0; i < pieces_.size(); ++i) {
            Piece* self = pieces_[i];
            for (size_t j = 0; j < enemy.pieces_.size(); ++j) {
                Piece* other = enemy.pieces_[j];
                int dist = abs(self->getX() - other->getX()) + abs(self->getY() - other->getY());
                if (dist <= self->getVision()) {
                    cout << "  " << other->getName() << " at (" << other->getX() << "," << other->getY() << ")" << endl;
                }
            }
        }
    }

private:
    string name_;
    int gold_;
    vector<Piece*> pieces_;
};

class Game {
public:
    Game() : player1_("玩家1", 15), player2_("玩家2", 10) {}

    void init() {
        player1_.buyPiece(new King(), map_, 0, 0);
        player1_.buyPiece(new Bow(), map_, 1, 0);

        player2_.buyPiece(new Doctor(), map_, 2, 2);
        player2_.buyPiece(new Sword(), map_, 3, 2);
    }

    void run() {
        cout << "---- 回合1 ----" << endl;
        player1_.showVisibleEnemies(player2_);
        player2_.showVisibleEnemies(player1_);

        cout << "\n玩家2的Doctor移动到(1,1)" << endl;
        if (!player2_.pieces_.empty()) {
            player2_.pieces_[0]->setPosition(1, 1);
        }

        cout << "---- 回合2 ----" << endl;
        player1_.showVisibleEnemies(player2_);
        player2_.showVisibleEnemies(player1_);

        cout << "\n玩家1购买新的Sword到(2,1)" << endl;
        player1_.buyPiece(new Sword(), map_, 2, 1);

        cout << "---- 回合3 ----" << endl;
        player1_.showVisibleEnemies(player2_);
        player2_.showVisibleEnemies(player1_);

        cout << "\n最终状态:" << endl;
        map_.printPieces();
    }

private:
    Map map_;
    Player player1_;
    Player player2_;
};

int main() {
    Game game;
    game.init();
    game.run();
    return 0;
}
