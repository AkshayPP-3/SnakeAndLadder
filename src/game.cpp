#include "game.hpp"

// Constructor
Game::Game()
    : player1("Player 1"),
      player2("Player 2"),
      currentPlayer(0),
      gameOver(false)
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

void Game::playTurn() {
    if (gameOver) {
        return;
    }
    // Get the current player
    Player& player = getCurrentPlayer();

    // Roll dice
    int diceValue = dice.roll();

    // Starting rule: player must roll 1
    if (player.getPosition() == 0) {

        if (diceValue == 1) {
            player.setPosition(1);
        }

        switchTurn();
        return;
    }

    // Calculate new position
    int newPosition = player.getPosition() + diceValue;

    // Move only if position does not exceed 100
    if (newPosition <= 100) {

        // Check snake or ladder
        newPosition = checkPosition(newPosition);

        // Update player position
        player.setPosition(newPosition);
    }

    // Check winning condition
    if (checkWinner(player)) {
        gameOver = true;
        return;
    }

    // Switch to the next player
    switchTurn();
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

// Check whether game is over
bool Game::isGameOver() const {

    return gameOver;
}
