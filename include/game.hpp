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
    int lastDiceValue;

public:
    Game();

    int checkPosition(int position);
    void playTurn();
    void switchTurn();
    Player& getCurrentPlayer();

    bool checkWinner(const Player& player);
    bool isGameOver() const;

    Player& getPlayer1();
    Player& getPlayer2();

    int getCurrentPlayerNumber() const;
    int getLastDiceValue() const;
};
