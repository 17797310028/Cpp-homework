#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Piece {
public:
    Piece(const std::string& name, int hp, int attack)
        : name_(name), hp_(hp), attack_(attack) {}
    virtual ~Piece() = default;

    const std::string& getName() const { return name_; }
    int getHP() const { return hp_; }
    int getAttack() const { return attack_; }

    void setHP(int hp) { hp_ = hp; }
    void setAttack(int attack) { attack_ = attack; }

private:
    std::string name_;
    int hp_;
    int attack_;
};

class King : public Piece { public: King() : Piece("King", 150, 20) {} };
class Doctor : public Piece { public: Doctor() : Piece("Doctor", 120, 10) {} };
class Bow : public Piece { public: Bow() : Piece("Bow", 100, 15) {} };
class Sword : public Piece { public: Sword() : Piece("Sword", 110, 18) {} };

class Map {
public:
    Map(int width, int height) : width_(width), height_(height), grid_(height) {
        for (auto& row : grid_) row.resize(width_);
    }
    void placePiece(std::unique_ptr<Piece> p, int x, int y) {
        if (x>=0 && x<width_ && y>=0 && y<height_) grid_[y][x] = std::move(p);
    }
    Piece* getPiece(int x, int y) const {
        if (x>=0 && x<width_ && y>=0 && y<height_) return grid_[y][x].get();
        return nullptr;
    }
private:
    int width_;
    int height_;
    std::vector<std::vector<std::unique_ptr<Piece>>> grid_;
};

class Game {
public:
    Game(int width, int height) : map_(width, height), turn_(0) {
        map_.placePiece(std::make_unique<King>(), 0, 0);
        map_.placePiece(std::make_unique<Doctor>(), 1, 0);
        map_.placePiece(std::make_unique<Bow>(), 0, 1);
        map_.placePiece(std::make_unique<Sword>(), 1, 1);
    }
    void run(int turns) {
        for (int i=0;i<turns;++i) {
            ++turn_;
            std::cout << "Turn: " << turn_ << std::endl;
            // simple log of pieces
            for (int y=0; y<2; ++y) {
                for (int x=0; x<2; ++x) {
                    Piece* p = map_.getPiece(x,y);
                    if (p) std::cout << p->getName() << "("<< p->getHP() <<") ";
                }
            }
            std::cout << std::endl;
        }
    }
private:
    Map map_;
    int turn_;
};

int main(){
    Game game(8,8);
    game.run(3);
    return 0;
}
