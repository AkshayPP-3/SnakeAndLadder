
#pragma once

#include "player.hpp"
#include "dice.hpp"

struct SnakeLadder {
    int start;
    int end;
};

class Game {
private:
    SnakeLadder ladders[8];
    SnakeLadder snakes[9];

    Player player1;
    Player player2;

    int currentPlayer;
    bool gameOver;

    Dice dice;

public:
    Game();

    int checkPosition(int position);
    void playTurn(Player& player, Dice& dice);
    bool checkWinner(const Player& player);
};
