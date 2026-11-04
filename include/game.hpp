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

    Dice dice;

    int currentPlayer;
    bool gameOver;

public:
    Game();

    int checkPosition(int position);
    void playTurn();
    bool checkWinner(const Player& player);
    void switchTurn();
    Player& getCurrentPlayer();
    bool isGameOver() const;
};
