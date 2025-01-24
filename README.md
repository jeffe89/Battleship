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

### `void welcome_screen(ofstream& bsLog)`
Displays a welcome message and instructions for the player.
- `bsLog`: File stream for logging game events.

### `void init_board(char gameBoard[BOARD_SIZE][BOARD_SIZE])`
Initializes the game board by filling it with `'-'` to represent empty spaces.
- **Parameters:**
  - `gameBoard[BOARD_SIZE][BOARD_SIZE]`: The game board array to initialize.

### `void init_board_manual(ofstream& bsLog, char p1Board[BOARD_SIZE][BOARD_SIZE])`
Allows the player to manually place ships on their board.
- **Parameters:**
- - `bsLog`: File stream for logging game events.
  - `p1Board[BOARD_SIZE][BOARD_SIZE]`: The player's game board array.
 
### `void init_board_read(ofstream& bsLog, string file_name, char p1Board[BOARD_SIZE][BOARD_SIZE])`
Allows the player to place ships on their board according to text file input.
- **Parameters:**
  - `bsLog`: File stream for logging game events.
  - `file_name`: Text file name to pull data on ship positions.
  - `p1Board[BOARD_SIZE][BOARD_SIZE]`: The player's game board array.

### `void init_board_random(char p1Board[BOARD_SIZE][BOARD_SIZE])`
Randomly places ships on the board for either the player or the CPU.
- **Parameters:**
  - `p1Board[BOARD_SIZE][BOARD_SIZE]`: The game board array.

### `void p1update(ofstream& bsLog, char p2Board[BOARD_SIZE][BOARD_SIZE], char p2HiddenBoard[BOARD_SIZE][BOARD_SIZE], int row, int col, Player& p1, Player& p2)`
Updates the player's board based on the result of an attack and logs the event to a file.
- **Parameters:**
  - `bsLog`: File stream for logging game events.
  - `p2Board[BOARD_SIZE][BOARD_SIZE]`: The cpu's displayed game board array.
  - `char p2HiddenBoard[BOARD_SIZE][BOARD_SIZE]`: The cpu's hidden game board array.
  - `row`: Row index to update.
  - `col`: Column index to update.
  - `Player& p1`: Player 1 struct for endgame statistics.
  - `Player& p2`: Player 2 (CPU) struct for endgame statistics.
  

### `void p2update(ofstream& bsLog, char p1Board[BOARD_SIZE][BOARD_SIZE], int row, int col, Player& p1, Player& p2)`
Updates the CPU's board based on the result of an attack and logs the event to a file.
- **Parameters:**
  - `bsLog`: File stream for logging game events.
  - `p1Board[BOARD_SIZE][BOARD_SIZE]`: The player's game board array.
  - `row`: Row index to update.
  - `col`: Column index to update.
  - `Player& p1`: Player 1 struct for endgame statistics.
  - `Player& p2`: Player 2 (CPU) struct for endgame statistics.

### `bool is_winner(char p1Board[BOARD_SIZE][BOARD_SIZE])`
Determines if the player or CPU has won the game.
- **Parameters:**
  - `p1Board[BOARD_SIZE][BOARD_SIZE]`: The game board array.
- **Returns:**
  - `true` if the board belongs to the winner.

### `bool check_if_sunk(ofstream& bsLog, Player& player)`
Checks if a ship represented by `shipChar` is completely sunk.
- **Parameters:**
  - `bsLog`: File stream for logging game events.
  - `player`: Player structure containing ship information.
- **Returns:**
  - `true` if the ship is completely sunk, `false` otherwise.

### `void display_single_board(char gameBoard[BOARD_SIZE][BOARD_SIZE])`
Displays a single board without hiding any ships.
- **Parameters:**
  - `gameBoard[BOARD_SIZE][BOARD_SIZE]]`: The game board array.

### `void display_both_boards(char p1Board[BOARD_SIZE][BOARD_SIZE], char p2Board[BOARD_SIZE][BOARD_SIZE])`
Displays both the player and CPU boards, hiding CPU ships.
- **Parameters:**
  - `p1Board[BOARD_SIZE][BOARD_SIZE]`: The player's game board.
  - `p2Board[BOARD_SIZE][BOARD_SIZE]`: The CPU's game board.

### `void outputStats(ofstream& bsLog, Player& p1, Player& p2)`
Outputs the game statistics, including total hits, misses, and ships sunk, and logs the statistics to a file.
- **Parameters:**
  - `bsLog`: File stream for logging game events.
  - `p1`: Player 1 structure containing game statistics.
  - `p2`: Player 2 structure containing game statistics.

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
