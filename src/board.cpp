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

            //board
            int x = startX + col * cellsSize;
            int y = startY + row * cellsSize;
            if((row+col)%2==0){
                DrawRectangle(x,y,cellsSize,cellsSize,lightCream);
            }
            else{
                DrawRectangle(x,y,cellsSize,cellsSize,goldenYellow);
            }
            DrawRectangleLines(x,y,cellsSize,cellsSize,warmBrown);

            //text
             int boardRow = 9- row;
             int number;
            if(boardRow%2==0){
                number = boardRow*10+col+1;
            }else{
                number = boardRow*10+(10-col);
            }
            const char* text = TextFormat("%d", number);
            int fontSize = 18;
            int textWidth = MeasureText(text, fontSize);
            int textX = x + (cellsSize - textWidth) / 2;
            int textY = y + (cellsSize - fontSize) / 2;
            DrawText(text, textX, textY, fontSize, DARKBROWN);
        }
    }
}

Vector2 GetCellCenter(int position)
{
    int cellSize = 50;
    int startX = 150;
    int startY = 50;

    int row = (position - 1) / 10;
    int col = (position - 1) % 10;

    // Reverse column direction for alternate rows
    if (row % 2 == 1)
    {
        col = 9 - col;
    }

    int x = startX + col * cellSize + cellSize / 2;
    int y = startY + (9 - row) * cellSize + cellSize / 2;

    return {(float)x, (float)y};
}