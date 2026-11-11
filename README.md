# Snake and Ladder

A simple two-player Snake and Ladder desktop game built with C++ and
[raylib](https://www.raylib.com/). The game includes animated pawns, dice
rolling, snakes and ladders, a winner animation, and a play-again flow.

## Gameplay Preview

![Snake and Ladder gameplay](screenshot.png)

## Features

- Two-player local gameplay
- 10 x 10 numbered board
- Animated dice and pawn movement
- 9 snakes and 8 ladders
- Extra turn after rolling a 6
- Winner animation with a play-again option
- Custom background, player, snake, ladder, dice, and UI artwork

## How to Play

1. Launch the game.
2. Player 1 rolls first by clicking the die on the left.
3. Player 2 rolls by clicking the die on the right when it is their turn.
4. A player must roll a **1** to enter the board.
5. Landing on the bottom of a ladder moves the player up.
6. Landing on the head of a snake moves the player down.
7. Players cannot move beyond square 100.
8. The first player to reach square 100 wins.
9. Rolling a 6 grants another turn. After three consecutive 6s, the turn
   changes to the other player.
10. After a winner is shown, use the play-again control to start a new game.

## Requirements

- Windows
- C++17-compatible compiler
- [raylib](https://www.raylib.com/) for graphics and window handling
- The project is currently configured for the raylib Windows toolchain at:
  `C:\raylib\w64devkit`

## Build and Run

### Using VS Code

1. Open this folder in VS Code.
2. Make sure raylib is installed at `C:\raylib\w64devkit`.
3. Open the **Terminal** menu and run the default build task, or press
   `Ctrl+Shift+B`.
4. Run the **Run Snake and Ladder** task from the Command Palette.

The included VS Code tasks compile the project with C++17 and link the
required Windows libraries:

- `raylib`
- `opengl32`
- `gdi32`
- `winmm`

### Using the compiler directly

From the project root, run:

```powershell
C:\raylib\w64devkit\bin\g++.exe -std=c++17 `
  -o SnakeAndLadder.exe `
  src\main.cpp src\board.cpp src\game.cpp src\dice.cpp `
  src\player.cpp src\renderer.cpp src\movement.cpp `
  -I include `
  -I C:\raylib\w64devkit\include `
  -L C:\raylib\w64devkit\lib `
  -lraylib -lopengl32 -lgdi32 -lwinmm
```

Then start the game:

```powershell
.\SnakeAndLadder.exe
```

Run the executable from the project root so the relative asset paths resolve
correctly.

## Architecture Overview

- `src/main.cpp` manages the application loop, input, animation updates, and
  drawing.
- `src/game.cpp` contains turn handling, movement rules, snakes, ladders, and
  win detection.
- `src/board.cpp` draws the numbered board.
- `src/movement.cpp` animates pawn movement and snake/ladder transitions.
- `src/renderer.cpp` loads assets and renders the game scene and winner
  animation.
- `include/` contains the public interfaces for the game components.
- `assets/` contains the artwork used by the renderer.

## Notes

- Asset paths are relative to the working directory.
- The game window is 800 x 600 pixels.
- The checked-in executable is intended for Windows and may need to be rebuilt
  after source changes.
