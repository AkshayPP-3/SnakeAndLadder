
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

