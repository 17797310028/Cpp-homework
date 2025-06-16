#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cstdio>

using namespace std;

struct Position {
    int x;
    int y;
};

class Player; // forward declaration

class Piece {
    friend class Player; // 允许 Player 修改名称等私有成员
public:
    Piece(const string& name, int hp, int attack, int cost, int vision,
          int speed, int range, int x = 0, int y = 0)
        : name_(name), hp_(hp), attack_(attack), cost_(cost), vision_(vision),
          speed_(speed), range_(range), pos_{x, y} {}
    virtual ~Piece() = default;

    const std::string& getName() const { return name_; }
    int getHP() const { return hp_; }
    int getAttack() const { return attack_; }
    int getX() const { return pos_.x; }
    int getY() const { return pos_.y; }
    int getCost() const { return cost_; }
    int getVision() const { return vision_; }
    int getSpeed() const { return speed_; }
    int getRange() const { return range_; }

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
    int speed_;
    int range_;
    Position pos_;
}; 

class King : public Piece {
public:
    King() : Piece("King", 200, 20, 5, 3, 1, 3) {}
};

class Doctor : public Piece {
public:
    Doctor() : Piece("Doctor", 120, 5, 4, 2, 2, 1) {}
};

class Bow : public Piece {
public:
    Bow() : Piece("Bow", 100, 15, 3, 2, 1, 3) {}
};

class Sword : public Piece {
public:
    Sword() : Piece("Sword", 110, 25, 4, 1, 1, 1) {}
};

class Game;  // forward declaration

class Map {
    friend class Game; // 让 Game 直接访问棋盘
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 10;
    ~Map() {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            delete pieces_[i];
        }
    }

    void addPiece(Piece* p) {
        pieces_.push_back(p);
    }

    bool inBounds(int x, int y) const {
        return x >= 1 && x <= WIDTH && y >= 1 && y <= HEIGHT;
    }

    bool isOccupied(int x, int y) const {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            if (pieces_[i]->getX() == x && pieces_[i]->getY() == y) {
                return true;
            }
        }
        return false;
    }

    Piece* getPieceAt(int x, int y) const {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            if (pieces_[i]->getX() == x && pieces_[i]->getY() == y) {
                return pieces_[i];
            }
        }
        return NULL;
    }

    void removePiece(Piece* p) {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            if (pieces_[i] == p) {
                pieces_.erase(pieces_.begin() + i);
                delete p;
                break;
            }
        }
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
    Player(const string& name, int gold, int bx, int by)
        : name_(name), gold_(gold), baseX_(bx), baseY_(by) {}

    bool buyPiece(Piece* p, Map& map, int x, int y) {
        int cost = p->getCost();
        if (gold_ < cost) {
            cout << name_ << " 金币不足，无法购买 " << p->getName() << endl;
            delete p;
            return false;
        }
        if (!map.inBounds(x, y)) {
            cout << "坐标越界，无法购买" << endl;
            delete p;
            return false;
        }
        if (abs(x - baseX_) > 2 || abs(y - baseY_) > 2) {
            cout << "只能在大本营周围2格内放置" << endl;
            delete p;
            return false;
        }
        if (map.isOccupied(x, y)) {
            cout << "目标格已有棋子" << endl;
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

    bool movePiece(size_t index, int x, int y, Map& map) {
        if (index >= pieces_.size()) {
            cout << "无效的棋子序号" << endl;
            return false;
        }
        Piece* p = pieces_[index];
        int dist = abs(p->getX() - x) + abs(p->getY() - y);
        if (dist > p->getSpeed()) {
            cout << "超出移动范围" << endl;
            return false;
        }
        if (!map.inBounds(x, y)) {
            cout << "坐标越界" << endl;
            return false;
        }
        if (map.isOccupied(x, y)) {
            cout << "目标格已有棋子" << endl;
            return false;
        }
        p->setPosition(x, y);
        return true;
    }

    void removePiece(Piece* p) {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            if (pieces_[i] == p) {
                pieces_.erase(pieces_.begin() + i);
                break;
            }
        }
    }

    const vector<Piece*>& getPieces() const { return pieces_; }

    const string& getName() const { return name_; }

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
    int baseX_;
    int baseY_;
};

class Game {
public:
    Game() : player1_("玩家1", 20, 1, 1), player2_("玩家2", 20, 10, 10) {}

    void init() {
        player1_.buyPiece(new King(), map_, 1, 1);
        player1_.buyPiece(new Bow(), map_, 2, 1);

        player2_.buyPiece(new Doctor(), map_, 10, 10);
        player2_.buyPiece(new Sword(), map_, 9, 10);
    }

    void attack(Piece* attacker, Player& enemy) {
        for (size_t i = 0; i < enemy.getPieces().size(); ++i) {
            Piece* target = enemy.getPieces()[i];
            int dist = abs(attacker->getX() - target->getX()) +
                       abs(attacker->getY() - target->getY());
            if (dist <= attacker->getRange()) {
                target->setHP(target->getHP() - attacker->getAttack());
                cout << attacker->getName() << " 攻击 " << target->getName()
                     << " 剩余HP:" << target->getHP() << endl;
                if (target->getHP() <= 0) {
                    cout << target->getName() << " 被击败" << endl;
                    map_.removePiece(target);
                    enemy.removePiece(target);
                    --i;
                }
            }
        }
    }

    void heal(Piece* doctor, Player& self) {
        for (size_t i = 0; i < self.getPieces().size(); ++i) {
            Piece* ally = self.getPieces()[i];
            int dx = abs(doctor->getX() - ally->getX());
            int dy = abs(doctor->getY() - ally->getY());
            if (dx <= 1 && dy <= 1 && ally != doctor) {
                ally->setHP(ally->getHP() + 10);
                cout << doctor->getName() << " 治疗 " << ally->getName()
                     << " HP:" << ally->getHP() << endl;
            }
        }
    }

    void playerTurn(Player& player, Player& enemy) {
        cout << "\n" << player.getName() << " 的回合" << endl;
        const vector<Piece*>& myPieces = player.getPieces();
        for (size_t i = 0; i < myPieces.size(); ++i) {
            Piece* p = myPieces[i];
            cout << i << ":" << p->getName() << " (" << p->getX() << "," << p->getY()
                 << ")" << endl;
        }
        size_t index; int x; int y;
        cout << "选择棋子序号和目标坐标 x y: ";
        if (!(cin >> index >> x >> y)) {
            cout << "输入错误" << endl;
            return;
        }
        if (!player.movePiece(index, x, y, map_)) {
            return;
        }

        Piece* moved = player.getPieces()[index];
        if (dynamic_cast<Doctor*>(moved)) {
            heal(moved, player);
        } else {
            attack(moved, enemy);
        }
    }

    void run() {
        for (int t = 1; t <= 1; ++t) {
            cout << "\n----- 回合 " << t << " -----" << endl;
            playerTurn(player1_, player2_);
            playerTurn(player2_, player1_);
        }

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
