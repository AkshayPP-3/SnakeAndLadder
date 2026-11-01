#include "board.hpp"
#include "raylib.h"
void DrawBoard(){
    int cellsSize = 50;
    int startX = 150;
    int startY = 50;
    Color lightCream = {253, 243, 205, 255};
    Color goldenYellow = {253, 229, 140, 255};
    Color warmBrown = {176, 139, 83, 255};
    for(int row=0;row<10;row++){
        for(int col=0;col<10;col++){
            int x = startX + col * cellsSize;
            int y = startY + row * cellsSize;
            if((row+col)%2==0){
                DrawRectangle(x,y,cellsSize,cellsSize,lightCream);
            }
            else{
                DrawRectangle(x,y,cellsSize,cellsSize,goldenYellow);
            }
            DrawRectangleLines(x,y,cellsSize,cellsSize,warmBrown);
        }
    }
}