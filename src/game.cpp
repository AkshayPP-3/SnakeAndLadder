#include "game.hpp"

// Constructor
Game::Game()
    : player1("Player 1"),
      player2("Player 2"),
      currentPlayer(0),
      gameOver(false),
      lastDiceValue(0),
      consecutiveSixes(0)
{
    // Initialize ladder positions
    ladders[0] = {2, 18};
    ladders[1] = {9, 28};
    ladders[2] = {25, 47};
    ladders[3] = {30, 50};
    ladders[4] = {45, 57};
    ladders[5] = {51, 73};
    ladders[6] = {65, 85};
    ladders[7] = {79, 98};

    // Initialize snake positions
    snakes[0] = {27, 5};
    snakes[1] = {38, 17};
    snakes[2] = {52, 33};
    snakes[3] = {66, 46};
    snakes[4] = {90, 70};
    snakes[5] = {93, 67};
    snakes[6] = {95, 75};
    snakes[7] = {97, 58};
    snakes[8] = {99, 81};
}

int Game::checkPosition(int position) {
    // Check ladders
    for (int i=0;i<8;i++) {
        if (position == ladders[i].start) {
            return ladders[i].end;
        }
    }

    // Check snakes
    for (int i = 0;i<9;i++) {
        if (position == snakes[i].start) {
            return snakes[i].end;
        }
    }
    return position;
}

// Plays one turn using the dice value that was shown on screen.
// The returned TurnResult tells the animation code what to move.
TurnResult Game::playTurn(int diceValue) {
    TurnResult result = {getCurrentPlayerNumber(), 0, 0, 0, false, false};

    if (gameOver) {
        return result;
    }
    // Get the current player
    Player& player = getCurrentPlayer();

    lastDiceValue = diceValue;

    result.from = player.getPosition();
    result.landed = result.from;
    result.to = result.from;

    // Starting rule: player must roll 1
    if (player.getPosition() == 0) {

        if (diceValue == 1) {
            player.setPosition(1);

            result.landed = 1;
            result.to = 1;
            result.moved = true;

            // Entering the board with a 1 gives one extra turn (only this first step)
            consecutiveSixes = 0;
            result.extraTurn = true;
            return result;
        }

        result.extraTurn = finishTurn(diceValue);
        return result;
    }

    // Calculate new position
    int newPosition = player.getPosition() + diceValue;

    // Move only if position does not exceed 100
    if (newPosition <= 100) {

        result.landed = newPosition;

        // Check snake or ladder
        newPosition = checkPosition(newPosition);

        // Update player position
        player.setPosition(newPosition);

        result.to = newPosition;
        result.moved = true;
    }

    // Check winning condition
    if (checkWinner(player)) {
        gameOver = true;
        return result;
    }

    // Switch to the next player (or give an extra turn after a 6)
    result.extraTurn = finishTurn(diceValue);
    return result;
}

// Rolling a 6 gives the same player another turn.
// A 6 on the first and second roll gives another turn, the third 6 in a row
// still moves the pawn but the turn goes to the opponent.
// Returns true if the same player plays again.
bool Game::finishTurn(int diceValue) {

    if (diceValue == 6) {
        consecutiveSixes++;

        if (consecutiveSixes < 3) {
            return true;
        }
    }

    consecutiveSixes = 0;
    switchTurn();
    return false;
}

// Check whether player has won
bool Game::checkWinner(const Player& player) {

    return player.getPosition() == 100;
}

// Get current player
Player& Game::getCurrentPlayer() {

    if (currentPlayer == 0) {
        return player1;
    }

    return player2;
}

// Switch between players
void Game::switchTurn() {

    if (currentPlayer == 0) {
        currentPlayer = 1;
    }
    else {
        currentPlayer = 0;
    }
}

Player& Game::getPlayer1() {
    return player1;
}

Player& Game::getPlayer2() {
    return player2;
}

int Game::getCurrentPlayerNumber() const {
    return currentPlayer + 1;
}

int Game::getLastDiceValue() const {
    return lastDiceValue;
}

bool Game::isGameOver() const {
    return gameOver;
}