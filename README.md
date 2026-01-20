# Minesweeper Game in C

A console-based implementation of the classic **Minesweeper** game written in C.  
The game features dynamic board creation, mine placement, flagging, and automatic revealing of empty cells. Players can save their scores to a file along with their name and time taken.

---

## Features

- Dynamic board size and mine count chosen by the player.
- Flagging system to mark suspected mines.
- Automatic reveal of neighboring empty cells.
- Win and loss detection.
- Saves scores to a `scores.txt` file with player name and time.
- Fully console-based, cross-platform.

---

## How to Play

1. Compile the game:
   ```bash
   gcc minesweeper.c -o minesweeper
Run the game:

./minesweeper
Enter the number of rows, columns, and mines when prompted.

Controls:

Reveal a cell: r <row> <col>

Flag or unflag a cell: f <row> <col>

The goal is to reveal all non-mine cells without triggering a mine.

After finishing, you can enter your name to save your score and time in scores.txt.

Code Structure
Cell struct: Represents each cell in the board with mine info, number of adjacent mines, and current state (hidden, revealed, flagged).

Dynamic field creation: Memory allocated dynamically to allow variable board sizes.

Flood-fill algorithm: Reveals empty neighboring cells automatically.

Win detection: Checks if all safe cells are revealed.

Score saving: Appends player name, score, and time to scores.txt.

Dependencies
Standard C libraries:

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
No external libraries are required.

Example Output
========================================
        M I N E S W E E P E R
========================================
Welcome to the classic game of Minesweeper!

Rules:
 1. The board is filled with hidden mines.
 2. Your goal is to reveal all safe cells without hitting a mine.
 3. Each revealed cell shows the number of mines in the 8
    surrounding cells. A '0' means no adjacent mines.
 4. You can flag suspected mine cells to avoid clicking them.

Controls:
  r <row> <col>  -> Reveal cell at (row, col)
  f <row> <col>  -> Flag/unflag cell at (row, col)

Good luck, and watch your step!
========================================
License
This project is open-source and free to use. Feel free to modify or distribute it as needed.
