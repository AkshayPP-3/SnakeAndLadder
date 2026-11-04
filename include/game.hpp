#include "player.hpp"
#include "dice.hpp"

#pragma once

struct SnakeLadder {
    int start;
    int end;
};

class Game {
private:
    SnakeLadder ladders[8];
    SnakeLadder snakes[9];

public:
    Game();
    int checkPosition(int position);
    void playTurn(const Player& player, Dice& dice);
    bool checkWinner(Player& player);
};