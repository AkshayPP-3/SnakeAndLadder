
#pragma once

#include <string>
using namespace std;

class Player {
private:
    string name;
    int position;

public:
    Player(string playerName);

    void move(int steps);
    int getPosition() const;
    string getName() const;
    void setPosition(int newPosition);
};
