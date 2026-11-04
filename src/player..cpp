#include "player.hpp"

Player::Player(string playerName){
    name = playerName;
    position = 0;
}
void Player::move(int steps){
    position += steps;
}
int Player::getPosition(){
    return position;
}
string Player::getName(){
    return name;
}
void Player::setPosition(int newPosition){
    position = newPosition;
}
