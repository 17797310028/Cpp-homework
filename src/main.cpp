#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iomanip>
#include <cstring>

using namespace std;

#define COLOR_RED "\033[31m"
#define COLOR_WHITE "\033[37m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

struct Stats {
    int hp;
    int attack;
    int cost;
    int vision;
    int speed;
    int range;
};

Stats kingStats{200,20,5,3,1,3};
Stats doctorStats{120,5,4,2,2,1};
Stats bowStats{100,15,3,2,1,3};
Stats swordStats{110,25,4,1,1,1};
Stats homeStats{300,15,0,3,0,2};

void saveStats() {
    FILE* f = fopen("stats.bin", "wb");
    if (!f) return;
    Stats arr[5] = {kingStats, doctorStats, bowStats, swordStats, homeStats};
    fwrite(arr, sizeof(Stats), 5, f);
    fclose(f);
}

void loadStats() {
    FILE* f = fopen("stats.bin", "rb");
    if (!f) {
        saveStats();
        return;
    }
    Stats arr[5];
    size_t r = fread(arr, sizeof(Stats), 5, f);
    fclose(f);
    if (r == 5) {
        kingStats = arr[0];
        doctorStats = arr[1];
        bowStats = arr[2];
        swordStats = arr[3];
        homeStats = arr[4];
    } else {
        saveStats();
    }
}

void modifyStats() {
    cout << "修改职业属性:" << endl;
    cout << "King hp attack cost vision speed range: "
         << kingStats.hp << ' ' << kingStats.attack << ' ' << kingStats.cost << ' '
         << kingStats.vision << ' ' << kingStats.speed << ' ' << kingStats.range << endl;
    cin >> kingStats.hp >> kingStats.attack >> kingStats.cost
        >> kingStats.vision >> kingStats.speed >> kingStats.range;

    cout << "Doctor hp attack cost vision speed range: "
         << doctorStats.hp << ' ' << doctorStats.attack << ' ' << doctorStats.cost << ' '
         << doctorStats.vision << ' ' << doctorStats.speed << ' ' << doctorStats.range << endl;
    cin >> doctorStats.hp >> doctorStats.attack >> doctorStats.cost
        >> doctorStats.vision >> doctorStats.speed >> doctorStats.range;

    cout << "Bow hp attack cost vision speed range: "
         << bowStats.hp << ' ' << bowStats.attack << ' ' << bowStats.cost << ' '
         << bowStats.vision << ' ' << bowStats.speed << ' ' << bowStats.range << endl;
    cin >> bowStats.hp >> bowStats.attack >> bowStats.cost
        >> bowStats.vision >> bowStats.speed >> bowStats.range;

    cout << "Sword hp attack cost vision speed range: "
         << swordStats.hp << ' ' << swordStats.attack << ' ' << swordStats.cost << ' '
         << swordStats.vision << ' ' << swordStats.speed << ' ' << swordStats.range << endl;
    cin >> swordStats.hp >> swordStats.attack >> swordStats.cost
        >> swordStats.vision >> swordStats.speed >> swordStats.range;

    cout << "Home hp attack cost vision range(速度固定0): "
         << homeStats.hp << ' ' << homeStats.attack << ' ' << homeStats.cost << ' '
         << homeStats.vision << ' ' << homeStats.range << endl;
    cin >> homeStats.hp >> homeStats.attack >> homeStats.cost
        >> homeStats.vision >> homeStats.range;

    saveStats();
}

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
    King() : Piece("King", kingStats.hp, kingStats.attack, kingStats.cost,
                   kingStats.vision, kingStats.speed, kingStats.range) {}
};

class Doctor : public Piece {
public:
    Doctor() : Piece("Doctor", doctorStats.hp, doctorStats.attack, doctorStats.cost,
                     doctorStats.vision, doctorStats.speed, doctorStats.range) {}
};

class Bow : public Piece {
public:
    Bow() : Piece("Bow", bowStats.hp, bowStats.attack, bowStats.cost,
                   bowStats.vision, bowStats.speed, bowStats.range) {}
};

class Sword : public Piece {
public:
    Sword() : Piece("Sword", swordStats.hp, swordStats.attack, swordStats.cost,
                    swordStats.vision, swordStats.speed, swordStats.range) {}
};

class Home : public Piece {
public:
    Home() : Piece("Home", homeStats.hp, homeStats.attack, homeStats.cost,
                   homeStats.vision, homeStats.speed, homeStats.range) {}
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
        : name_(name), gold_(gold), baseX_(bx), baseY_(by), home_(NULL) {}

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

    bool ownsPiece(const Piece* p) const {
        for (size_t i = 0; i < pieces_.size(); ++i) {
            if (pieces_[i] == p) {
                return true;
            }
        }
        return false;
    }

    int getBaseX() const { return baseX_; }
    int getBaseY() const { return baseY_; }
    int getGold() const { return gold_; }

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
    Home* home_;
public:
    Home* getHome() const { return home_; }
    void setHome(Home* h) { home_ = h; pieces_.push_back(h); }
};

class Game {
public:
    Game(const string& n1, const string& n2)
        : player1_(n1, 5, 1, 1), player2_(n2, 5, 10, 10) {}

    char pieceSymbol(Piece* p) {
        if (dynamic_cast<King*>(p)) return 'K';
        if (dynamic_cast<Doctor*>(p)) return 'D';
        if (dynamic_cast<Bow*>(p)) return 'B';
        if (dynamic_cast<Sword*>(p)) return 'S';
        if (dynamic_cast<Home*>(p)) return 'H';
        return '?';
    }

    void showMap(const Player& self, const Player& enemy) {
        bool visible[Map::WIDTH + 1][Map::HEIGHT + 1];
        for (int x = 1; x <= Map::WIDTH; ++x) {
            for (int y = 1; y <= Map::HEIGHT; ++y) {
                visible[x][y] = false;
            }
        }
        for (size_t i = 0; i < self.getPieces().size(); ++i) {
            Piece* p = self.getPieces()[i];
            for (int x = 1; x <= Map::WIDTH; ++x) {
                for (int y = 1; y <= Map::HEIGHT; ++y) {
                    int dist = abs(p->getX() - x) + abs(p->getY() - y);
                    if (dist <= p->getVision()) {
                        visible[x][y] = true;
                    }
                }
            }
        }

        cout << "  ";
        for (int x = 1; x <= Map::WIDTH; ++x) {
            cout << x << ' ';
        }
        cout << endl;
        for (int y = 1; y <= Map::HEIGHT; ++y) {
            if (y < 10) cout << ' ' << y << ' ';
            else cout << y << ' ';
            for (int x = 1; x <= Map::WIDTH; ++x) {
                bool vis = visible[x][y];
                Piece* p = map_.getPieceAt(x, y);
                if (x == self.getBaseX() && y == self.getBaseY()) {
                    cout << COLOR_GREEN << 'H' << COLOR_RESET << ' ';
                } else if (x == enemy.getBaseX() && y == enemy.getBaseY() && vis) {
                    cout << COLOR_GREEN << 'h' << COLOR_RESET << ' ';
                } else if (p) {
                    if (self.ownsPiece(p)) {
                        cout << COLOR_WHITE << pieceSymbol(p) << COLOR_RESET << ' ';
                    } else if (vis) {
                        cout << COLOR_RED << pieceSymbol(p) << COLOR_RESET << ' ';
                    } else {
                        cout << "# ";
                    }
                } else {
                    if (vis)
                        cout << ". ";
                    else
                        cout << "# ";
                }
            }
            cout << endl;
        }
    }

    void init() {
        Home* h1 = new Home();
        h1->setName(player1_.getName() + "_Home");
        h1->setPosition(player1_.getBaseX(), player1_.getBaseY());
        map_.addPiece(h1);
        player1_.setHome(h1);

        Home* h2 = new Home();
        h2->setName(player2_.getName() + "_Home");
        h2->setPosition(player2_.getBaseX(), player2_.getBaseY());
        map_.addPiece(h2);
        player2_.setHome(h2);
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

    void buyPhase(Player& player) {
        cout << "是否购买棋子? 输入1为是, 其他为否: ";
        int op;
        if (!(cin >> op) || op != 1) {
            cin.clear();
            cin.ignore(1024, '\n');
            return;
        }
        while (true) {
            cout << "选择购买 1.King 2.Doctor 3.Bow 4.Sword 以及坐标x y: ";
            int t, x, y;
            if (!(cin >> t >> x >> y)) {
                cin.clear(); cin.ignore(1024, '\n'); break; }
            Piece* p = NULL;
            if (t == 1) p = new King();
            else if (t == 2) p = new Doctor();
            else if (t == 3) p = new Bow();
            else if (t == 4) p = new Sword();
            else { cout << "无效类型" << endl; continue; }
            player.buyPiece(p, map_, x, y);
            cout << "继续购买? 输入1继续, 其他结束: ";
            if (!(cin >> op) || op != 1) {
                cin.clear(); cin.ignore(1024, '\n');
                break;
            }
        }
    }

    void playerTurn(Player& player, Player& enemy) {
        cout << "\n" << player.getName() << " 的回合，金币:" << player.getGold() << endl;
        showMap(player, enemy);

        Home* home = player.getHome();
        bool canHomeAttack = false;
        for (size_t i = 0; i < enemy.getPieces().size(); ++i) {
            Piece* ep = enemy.getPieces()[i];
            int d = abs(home->getX() - ep->getX()) + abs(home->getY() - ep->getY());
            if (d <= home->getRange()) { canHomeAttack = true; break; }
        }
        if (canHomeAttack) {
            cout << "大本营可攻击，输入坐标x y (0 0跳过): ";
            int hx, hy; if (cin >> hx >> hy) {
                if (!(hx == 0 && hy == 0)) {
                    Piece* target = map_.getPieceAt(hx, hy);
                    if (target && !player.ownsPiece(target)) {
                        int dist = abs(home->getX()-hx)+abs(home->getY()-hy);
                        if (dist <= home->getRange()) {
                            target->setHP(target->getHP() - home->getAttack());
                            cout << home->getName() << " 攻击 " << target->getName()
                                 << " 剩余HP:" << target->getHP() << endl;
                            if (target->getHP() <= 0) {
                                cout << target->getName() << " 被击败" << endl;
                                map_.removePiece(target);
                                enemy.removePiece(target);
                            }
                        }
                    }
                }
            } else { cin.clear(); cin.ignore(1024,'\n'); }
        }

        buyPhase(player);

        const vector<Piece*>& myPieces = player.getPieces();
        for (size_t i = 0; i < myPieces.size(); ++i) {
            Piece* p = myPieces[i];
            if (dynamic_cast<Home*>(p)) continue;
            bool acted = false;
            while (!acted) {
                cout << "\n操作棋子 " << i << ":" << p->getName() << "(" << p->getX()
                     << "," << p->getY() << ") HP:" << p->getHP()
                     << " 选择 1.攻击 2.移动 3.跳过: ";
                int opAct; if (!(cin >> opAct)) { cin.clear(); cin.ignore(1024,'\n'); continue; }
                if (opAct == 1) {
                    int tx, ty; cout << "输入目标坐标x y: ";
                    if (!(cin >> tx >> ty)) { cin.clear(); cin.ignore(1024,'\n'); continue; }
                    Piece* target = map_.getPieceAt(tx, ty);
                    if (!target || player.ownsPiece(target)) { cout << "无效目标" << endl; continue; }
                    int dist = abs(p->getX()-tx) + abs(p->getY()-ty);
                    if (dist > p->getRange()) { cout << "超出攻击范围" << endl; continue; }
                    target->setHP(target->getHP() - p->getAttack());
                    cout << p->getName() << " 攻击 " << target->getName() << " 剩余HP:" << target->getHP() << endl;
                    if (target->getHP() <= 0) {
                        cout << target->getName() << " 被击败" << endl;
                        map_.removePiece(target);
                        enemy.removePiece(target);
                    }
                    if (dynamic_cast<Doctor*>(p)) heal(p, player);
                    acted = true;
                } else if (opAct == 2) {
                    int mx, my; cout << "输入目标坐标x y: ";
                    if (!(cin >> mx >> my)) { cin.clear(); cin.ignore(1024,'\n'); continue; }
                    if (player.movePiece(i, mx, my, map_)) acted = true; else continue;
                } else {
                    acted = true;
                }
            }
        }
    }

    void run() {
        for (int t = 1; t <= 5; ++t) {
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
    loadStats();
    while (true) {
        cout << "\n1.开始游戏  2.修改职业属性  3.退出游戏\n选择: ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(1024,'\n'); continue; }
        if (op == 1) {
            string n1, n2;
            cout << "输入玩家1名称: "; cin >> n1;
            cout << "输入玩家2名称: "; cin >> n2;
            Game game(n1, n2);
            game.init();
            game.run();
        } else if (op == 2) {
            modifyStats();
        } else if (op == 3) {
            break;
        }
    }
    return 0;
}
