#pragma once

#include "player.hpp"
#include "dice.hpp"

struct SnakeLadder {
    int start;
    int end;
};

// What happened during one turn (used by the animation code)
struct TurnResult {
    int player;     // 1 or 2
    int from;       // position before the roll (0 = outside the board)
    int landed;     // cell reached by the dice (before any snake / ladder)
    int to;         // final cell after snake / ladder (same as landed if none)
    bool moved;     // false if the pawn stayed where it was
    bool extraTurn; // true if the same player rolls again (they rolled a 6)
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
    int consecutiveSixes;

    // Called when a turn is over: gives an extra turn after a 6 (up to 3 sixes in a row)
    bool finishTurn(int diceValue);

public:
    Game();

    int checkPosition(int position);
    TurnResult playTurn(int diceValue);
    void switchTurn();
    Player& getCurrentPlayer();

    bool checkWinner(const Player& player);
    bool isGameOver() const;

    Player& getPlayer1();
    Player& getPlayer2();

    int getCurrentPlayerNumber() const;
    int getLastDiceValue() const;
};
