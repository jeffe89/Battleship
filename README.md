# Battleship Game Simulator

This repository contains a Battleship game implementation in C++. The program simulates a classic game of Battleship, where a single player competes against a CPU opponent. Below, you'll find detailed information about the project, including setup instructions, game mechanics, and a breakdown of all functions.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [How to Run](#how-to-run)
- [Game Rules](#game-rules)
- [Function Descriptions](#function-descriptions)
- [Future Enhancements](#future-enhancements)

## Introduction
The Battleship Game Simulator allows a single player to engage in a strategic guessing game against a CPU opponent. The program sets up a grid for both the player and the CPU, places ships, and handles the game logic as turns alternate between the player and the CPU.

## Features
- Single-player mode against a CPU opponent.
- Automatic or manual placement of ships.
- Turn-based gameplay.
- Feedback on hits, misses, and sunk ships.
- Endgame detection.

## Requirements
- C++ compiler (e.g., GCC, Clang).
- Standard C++11 or higher.

## How to Run
1. Clone this repository:
   ```bash
   git clone <repository-url>
   ```
2. Navigate to the project directory:
   ```bash
   cd battleship-simulator
   ```
3. Compile the program:
   ```bash
   g++ -std=c++11 battleship.cpp -o battleship
   ```
4. Run the executable:
   ```bash
   ./battleship
   ```

## Game Rules
1. Each player has a 10x10 grid.
2. Ships of varying sizes are placed randomly or manually on the grid.
3. Players take turns attacking a coordinate.
4. The program indicates whether the attack was a hit, miss, or sunk a ship.
5. The game ends when all ships of one player are sunk.

## Function Descriptions

### `void welcomeScreen()`
Displays a welcome message and instructions for the player.

### `void initBoard(char board[10][10])`
Initializes the game board by filling it with `'-'` to represent empty spaces.

### `void initBoardManual(char board[10][10])`
Allows the player to manually place ships on their board.

### `void initBoardRandom(char board[10][10])`
Randomly places ships on the board for either the player or the CPU.

### `bool placeShip(char board[10][10], int shipSize, char shipChar)`
Attempts to place a ship of size `shipSize` on the board using the character `shipChar`.
- **Parameters:**
  - `board`: The game board array.
  - `shipSize`: Size of the ship to place.
  - `shipChar`: Character representing the ship type.
- **Returns:**
  - `true` if the ship is successfully placed, `false` otherwise.

### `void printBoard(const char board[10][10], bool hideShips)`
Prints the current state of the board. If `hideShips` is true, ships are hidden.
- **Parameters:**
  - `board`: The game board array.
  - `hideShips`: Whether to hide ship positions.

### `void displaySingleBoard(const char board[10][10])`
Displays a single board without hiding any ships.
- **Parameters:**
  - `board`: The game board array.

### `void displayBothBoards(const char playerBoard[10][10], const char cpuBoard[10][10])`
Displays both the player and CPU boards, hiding CPU ships.
- **Parameters:**
  - `playerBoard`: The player's game board.
  - `cpuBoard`: The CPU's game board.

### `bool isGameOver(char board[10][10])`
Checks if all ships on the board have been sunk.
- **Parameters:**
  - `board`: The game board array.
- **Returns:**
  - `true` if all ships are sunk, `false` otherwise.

### `bool isWinner(const char board[10][10])`
Determines if the player or CPU has won the game.
- **Parameters:**
  - `board`: The game board array.
- **Returns:**
  - `true` if the board belongs to the winner.

### `bool attack(char opponentBoard[10][10], int row, int col)`
Handles an attack on the opponent's board at the given row and column.
- **Parameters:**
  - `opponentBoard`: The opponent's game board.
  - `row`: The row index of the attack.
  - `col`: The column index of the attack.
- **Returns:**
  - `true` if the attack is a hit, `false` otherwise.

### `void p1Update(char board[10][10], int row, int col, char result)`
Updates the player's board based on the result of an attack.
- **Parameters:**
  - `board`: The player's game board.
  - `row`: Row index to update.
  - `col`: Column index to update.
  - `result`: The result of the attack ('H' for hit, 'M' for miss).

### `void p2Update(char board[10][10], int row, int col, char result)`
Updates the CPU's board based on the result of an attack.
- **Parameters:**
  - `board`: The CPU's game board.
  - `row`: Row index to update.
  - `col`: Column index to update.
  - `result`: The result of the attack ('H' for hit, 'M' for miss).

### `bool checkIfSunk(char board[10][10], char shipChar)`
Checks if a ship represented by `shipChar` is completely sunk.
- **Parameters:**
  - `board`: The game board array.
  - `shipChar`: Character representing the ship type.
- **Returns:**
  - `true` if the ship is completely sunk, `false` otherwise.

### `void outputStats(int totalHits, int totalMisses, int shipsSunk)`
Outputs the game statistics, including total hits, misses, and ships sunk.
- **Parameters:**
  - `totalHits`: Total number of hits.
  - `totalMisses`: Total number of misses.
  - `shipsSunk`: Total number of ships sunk.

### `void startGame()`
Main function that initializes the game, handles player turns, and manages the game flow.

## Future Enhancements
- Add advanced AI for more challenging gameplay.
- Implement multiplayer support over a network.
- Include a graphical user interface.
- Add different difficulty levels.

Feel free to contribute to this project by submitting pull requests or reporting issues.

---

Thank you for checking out the Battleship Game Simulator! If you have any questions or feedback, please open an issue or reach out.
