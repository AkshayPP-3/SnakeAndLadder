#include "board.hpp"
#include "raylib.h"
void DrawBoard(){
    int cellsSize = 50;
    int startX = 150;
    int startY = 50;
    for(int row=0;row<10;row++){
        for(int col=0;col<10;col++){
            int x = startX + col * cellsSize;
            int y = startY + row * cellsSize;
            DrawRectangle(x,y,cellsSize,cellsSize,LIGHTGRAY);
            DrawRectangleLines(x,y,cellsSize,cellsSize,BLACK);
        }
    }
}