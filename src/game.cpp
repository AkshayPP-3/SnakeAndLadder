#include "game.hpp"

Game::Game() {

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

int Game::checkPosition(int position){
    for(int i=0;i<8;i++){
        if(position==ladders[i].start){
            return ladders[i].end;
        }
    }
    for(int i=0;i<9;i++){
        if(position==snakes[i].start){
            return snakes[i].end;
        }
    }
    return position;
}
void  Game::playTurn(Player& player, Dice& dice){
    int diceValue = dice.roll();

    //only move when dice shows 1 in the first step
    if (player.getPosition() == 0) {

        if (diceValue == 1) {
            player.setPosition(1);
        }

        return;
    }
    int currentPosition =player.getPosition();
    int newPosition = diceValue + currentPosition;
    
    if(newPosition>100) return;

    // Check for snake or ladder
    newPosition = checkPosition(newPosition);

    // Update player's position
    player.setPosition(newPosition);
}