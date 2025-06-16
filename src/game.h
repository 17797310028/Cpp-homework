#ifndef GAME_H
#define GAME_H

#include "map.h"

class Game {
public:
    Game(int width, int height);
    void run(int turns);

private:
    Map map_;
    int turn_;
};

#endif // GAME_H
