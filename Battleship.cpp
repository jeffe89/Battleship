//Geoffrey Giordano
//Lab #4 - Battleships 

/*	
	Battleship is a two player Navy game. The objective of the game is to sink all ships in your enemy's fleet.
	The Player to sink his/her enemy's fleet first wins. Both player's fleets consist of 5 ships that are hidden
	from the enemy. Each ship may be differentiated by its "size" (besides the Cruiser and Submarine) 
	or number of cells it expands on the game board. The Carrier has 5 cells, Battleship has 4 cells, Cruiser has 3 cells, Submarine has 3 cells, 
	and the	Destroyer has 2 cells.
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

#define BOARD_SIZE 10

struct Player {
	float total = 0;
	float hit = 0;
	float miss = 0;
	float hitPercent = 0;
	float missPercent = 0;
	float ships[5] = { 2, 3, 3, 4, 5 };
	int playerNum = 0;
};

int welcome_screen(ofstream& bsLog);																														// Display wlecome screen and rules of Battleship
void init_board(char gameBoard[BOARD_SIZE][BOARD_SIZE]);													// Initialize the board for default
void init_board_manual(ofstream& bsLog, char p1Board[BOARD_SIZE][BOARD_SIZE]);											// Initialize the board for manual placement
void init_board_read(ofstream& bsLog, string file_name, char p1Board[BOARD_SIZE][BOARD_SIZE]);									// Initialize the board for reading from a file
void init_board_random(char p1Board[BOARD_SIZE][BOARD_SIZE]);													// Initialize the board for random generation
void p1update(ofstream& bsLog, char p2Board[BOARD_SIZE][BOARD_SIZE], char p2HiddenBoard[BOARD_SIZE][BOARD_SIZE], int row, int col, Player& p1, Player& p2);	// Update board and determine if a shot was a hit or miss (1 for hit and -1 for miss) for player 1
void p2update(ofstream& bsLog, char p1Board[BOARD_SIZE][BOARD_SIZE], int row, int col, Player& p1, Player& p2);							// Update board and determine if a shot was a hit or miss (1 for hit and -1 for miss) for player 2
bool is_winner(char p1Board[BOARD_SIZE][BOARD_SIZE]);														// Determine if a winner exists
void check_if_sunk(ofstream& bsLog, Player& player);														// Determines if a player ship has sunk
void display_single_board(char gameBoard[BOARD_SIZE][BOARD_SIZE]);												// Display board to screen
void display_both_boards(char p1Board[BOARD_SIZE][BOARD_SIZE], char p2Board[BOARD_SIZE][BOARD_SIZE]);								// Display board to screen
void output_stats(ofstream& bsLog, Player& p1, Player& p2);													// Write the statistics collected for each player to the log file

int main()
{
	// Open the battleship log file
	ofstream bsLog;
	bsLog.open("battleship.log");

	// Initialize randomized seed
	srand(time(0));

	// Initialize variables
	int placeChoice = 0;
	char p1Board[BOARD_SIZE][BOARD_SIZE];
	char p2Board[BOARD_SIZE][BOARD_SIZE];
	char p2HiddenBoard[BOARD_SIZE][BOARD_SIZE];
	Player p1; p1.playerNum = 1;
	Player p2; p2.playerNum = 2;
	int row = 0;
	int col = 0;
	bool gameRun = true;

	// Initialize the three boards
	init_board(p1Board);
	init_board(p2Board);
	init_board(p2HiddenBoard);

	// Prompt the welcome screen and get the player placement choice
	placeChoice = welcome_screen(bsLog);

	// If the player chooses to place manually, go to init_board_manual
	if (placeChoice == 1) {
		init_board_manual(bsLog, p1Board);
	}
	// If the player chooses to place from a file, go to init_board_read
	else if (placeChoice == 2) {
		init_board_read(bsLog, "player_ships.txt", p1Board);
	}
	// If the player chooses to place randomly, go to init_board_random
	else if (placeChoice == 3) {
		init_board_random(p1Board);
	}

	// Initialize the computer board with init_board_random
	init_board_random(p2Board);

	display_both_boards(p1Board, p2HiddenBoard);

	while (gameRun) {
		// Reset for player 1 turn with invalid positioning for the shot
	player1turn:
		// Prompt the player for a row and column to fire at
		cout << "\nPLAYER1 - Enter a Row and a Column to fire at (0-9)";
		cout << "\nRow: ";
		cin >> row;
		cout << "Column: ";
		cin >> col;

		// bs.log update
		bsLog << "\nPLAYER1 - Enter a Row and a Column to fire at (0-9)";
		bsLog << "\nRow: ";
		bsLog << row;
		bsLog << " Column: ";
		bsLog << col;

		// If the row or column is out of bounds, print invalid input
		if (row > 9 || row < 0 || col > 9 || col < 0) {
			cout << "\nInvalid Input! Please Try Again!";
			bsLog << "\nInvalid Input! Please Try Again!";
			goto player1turn;
		}
		// If the player has already fired at the position on the board, print that they have already fired at it
		else if (p2Board[row][col] == 'm' || p2Board[row][col] == '*') {
			cout << "\nAlready fired at this spot! Please Try Again!";
			bsLog << "\nAlready fired at this spot! Please Try Again!";
			goto player1turn;
		}
		else {
			p1update(bsLog, p2Board, p2HiddenBoard, row, col, p1, p2);
			display_both_boards(p1Board, p2HiddenBoard);
		}

		gameRun = is_winner(p2Board);
		if (gameRun == false) {
			cout << "\nYou have sunk all of Player 2s ships! You won!\n";
			bsLog << "\nYou have sunk all of Player 2s ships! You won!\n";
			continue;
		}

	player2turn:
		row = rand() % 10;
		col = rand() % 10;

		// If the row or column is out of bounds, print invalid input
		if (row > 9 || row < 0 || col > 9 || col < 0) {
			goto player2turn;
		}
		// If the player has already fired at the position on the board, print that they have already fired at it
		else if (p1Board[row][col] == 'm' || p1Board[row][col] == '*') {
			goto player2turn;
		}
		else {
			p2update(bsLog, p1Board, row, col, p1, p2);
			display_both_boards(p1Board, p2HiddenBoard);
		}

		gameRun = is_winner(p2Board);
		if (gameRun == false) {
			cout << "\nPlayer 2 has sunk all of your ships! You lose!\n";
			bsLog << "\nPlayer 2 has sunk all of your ships! You lose!\n";
			continue;
		}
	}



	// Update the stats in bs.log and close the file for program exit
	output_stats(bsLog, p1, p2);
	bsLog.close();

	return 0;
}

int welcome_screen(ofstream& bsLog) {

	// Initialize variables
	int placement = 0;
	bool invalid = true;

	// Beginning prompt for user with how to play
	cout << "********** Welcome to Battleship! **********\n";
	cout << "How to Play:\n";
	cout << "1. Two players compete to sink each others battleships\n";
	cout << "2. Each player will take a turn, starting with Player 1, to determine the placement of their battleships\n";
	cout << "3. After the battleships have all been placed for both players, the players will then take turns guessing the opponents battleship locations\n";
	cout << "4. Once all of a players battleships have been sunk, they lose.\n";
	cout << "5. The grid size is 10 x 10, therefore coordinates for ships can only be between the digits 0-9\n\n";

	// bs.log update
	bsLog << "********** Welcome to Battleship! **********\n";
	bsLog << "How to Play:\n";
	bsLog << "1. Two players compete to sink each others battleships\n";
	bsLog << "2. Each player will take a turn, starting with Player 1, to determine the placement of their battleships\n";
	bsLog << "3. After the battleships have all been placed for both players, the players will then take turns guessing the opponents battleship locations\n";
	bsLog << "4. Once all of a players battleships have been sunk, they lose.\n";
	bsLog << "5. The grid size is 10 x 10, therefore coordinates for ships can only be between the digits 0-9\n\n";

	// Wait for user to press a key to continue and clear the console screen
	system("pause");
	system("CLS");

	// Prompt user for a '1' to manually place ships, '2' to read ship placement from file, or '3' for random generation
	cout << "\n\nPlease select from the following menu:\n";
	cout << "1. Enter positions of ships manually.\n";
	cout << "2. Import from a file.\n";
	cout << "3. Allow the program to randomly select positions of ships.\n";

	// bs.log update
	bsLog << "\n\nPlease select from the following menu:\n";
	bsLog << "1. Enter positions of ships manually.\n";
	bsLog << "2. Import from a file.\n";
	bsLog << "3. Allow the program to randomly select positions of ships.\n";

	// Store input for user selection and verify it is valid
	while (invalid) {
		cin >> placement;
		// If the user enters a '1' or a '2', break the while loop
		if (placement == 1 || placement == 2 || placement == 3) {
			break;
		}
		// Otherwise, return invalid input and have them re-enter a choice
		else {
			cout << "\nInvalid Input! Please try again.";
			bsLog << "\nInvalid Input! Please try again.";
		}
	}

	cout << endl;
	return placement;
}

void init_board(char gameBoard[BOARD_SIZE][BOARD_SIZE]) {

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			gameBoard[i][j] = '-';
		}
	}
}

void init_board_manual(ofstream& bsLog, char p1Board[BOARD_SIZE][BOARD_SIZE]) {


	// Initialize variables
	int shipCount = 5;
	int row = -1;
	int col = -1;
	int dir = 0;
	bool choosing = true;

	// Ship Placement
	while (choosing) {
		// Reset when incorrect placement and retry
	reset:
		row = -1;
		col = -1;
		dir = 0;

		// Carrier
		if (shipCount == 5) {
			// Prompt the user for a direction (up, down, left, right) and make sure input is valid
			cout << "\n********** CARRIER **********\n";
			cout << "1. Up\n";
			cout << "2. Down\n";
			cout << "3. Left\n";
			cout << "4. Right";
			// bs.log update
			bsLog << "\n********** CARRIER **********\n";
			bsLog << "1. Up\n";
			bsLog << "2. Down\n";
			bsLog << "3. Left\n";
			bsLog << "4. Right";
			while (dir > 4 || dir < 1) {
				cout << "\nPlease Enter the Direction: ";
				bsLog << "\nPlease Enter the Direction: ";
				cin >> dir;
				bsLog << dir;
				if (dir > 4 || dir < 1) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a row (0-9) and make sure input is valid
			while (row < 0 || row > 9) {
				cout << "\nEnter Row (0-9): ";
				bsLog << "\nEnter Row (0-9): ";
				cin >> row;
				bsLog << row;
				if (row < 0 || row > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a column (0-9) and make sure input is valid
			while (col < 0 || col > 9) {
				cout << "\nEnter Column (0-9): ";
				bsLog << "\nEnter Column (0-9): ";
				cin >> col;
				bsLog << col;
				if (col < 0 || col > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// For-loop to place the ships on the board and a switch statement to determine the direction
			for (int i = shipCount; i > 0; i--) {
				switch (dir) {

				// Up
				case 1:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row--][col] = 'c';
					break;

				// Down
				case 2:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row++][col] = 'c';
					break;
					
				// Left
				case 3:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col--] = 'c';
					break;

				// Right
				case 4:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col++] = 'c';
					break;
				}
			}
			// Decrement shipCount and move onto next ship
			shipCount--;
		}
		// Battleship
		else if (shipCount == 4) {
			// Prompt the user for a direction (up, down, left, right) and make sure input is valid
			cout << "\n********** BATTLESHIP **********\n";
			cout << "1. Up\n";
			cout << "2. Down\n";
			cout << "3. Left\n";
			cout << "4. Right";
			// bs.log update
			bsLog << "\n********** BATTLESHIP **********\n";
			bsLog << "1. Up\n";
			bsLog << "2. Down\n";
			bsLog << "3. Left\n";
			bsLog << "4. Right";
			while (dir > 4 || dir < 1) {
				cout << "\nPlease Enter the Direction: ";
				bsLog << "\nPlease Enter the Direction: ";
				cin >> dir;
				bsLog << dir;
				if (dir > 4 || dir < 1) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a row (0-9) and make sure input is valid
			while (row < 0 || row > 9) {
				cout << "\nEnter Row (0-9): ";
				bsLog << "\nEnter Row (0-9): ";
				cin >> row;
				bsLog << row;
				if (row < 0 || row > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a column (0-9) and make sure input is valid
			while (col < 0 || col > 9) {
				cout << "\nEnter Column (0-9): ";
				bsLog << "\nEnter Column (0-9): ";
				cin >> col;
				bsLog << col;
				if (col < 0 || col > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// For-loop to place the ships on the board and a switch statement to determine the direction
			for (int i = shipCount; i > 0; i--) {
				switch (dir) {

				// Up
				case 1:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row--][col] = 'b';
					break;

				// Down
				case 2:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row++][col] = 'b';
					break;

				// Left
				case 3:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col--] = 'b';
					break;

				// Right
				case 4:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col++] = 'b';
					break;
				}
			}
			// Decrement shipCount and move onto next ship
			shipCount--;
		}
		// Cruiser
		else if (shipCount == 3) {
			// Prompt the user for a direction (up, down, left, right) and make sure input is valid
			cout << "\n********** CRUISER **********\n";
			cout << "1. Up\n";
			cout << "2. Down\n";
			cout << "3. Left\n";
			cout << "4. Right";
			// bs.log update
			bsLog << "\n********** CRUISER **********\n";
			bsLog << "1. Up\n";
			bsLog << "2. Down\n";
			bsLog << "3. Left\n";
			bsLog << "4. Right";
			while (dir > 4 || dir < 1) {
				cout << "\nPlease Enter the Direction: ";
				bsLog << "\nPlease Enter the Direction: ";
				cin >> dir;
				bsLog << dir;
				if (dir > 4 || dir < 1) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a row (0-9) and make sure input is valid
			while (row < 0 || row > 9) {
				cout << "\nEnter Row (0-9): ";
				bsLog << "\nEnter Row (0-9): ";
				cin >> row;
				bsLog << row;
				if (row < 0 || row > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a column (0-9) and make sure input is valid
			while (col < 0 || col > 9) {
				cout << "\nEnter Column (0-9): ";
				bsLog << "\nEnter Column (0-9): ";
				cin >> col;
				bsLog << col;
				if (col < 0 || col > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// For-loop to place the ships on the board and a switch statement to determine the direction
			for (int i = shipCount; i > 0; i--) {
				switch (dir) {

				// Up
				case 1:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row--][col] = 'r';
					break;

				// Down
				case 2:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row++][col] = 'r';
					break;

				// Left
				case 3:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col--] = 'r';
					break;

				// Right
				case 4:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col++] = 'r';
					break;
				}
			}
			// Decrement shipCount and move onto next ship
			shipCount--;
		}
		// Submarine
		else if (shipCount == 2) {
			// Prompt the user for a direction (up, down, left, right) and make sure input is valid
			cout << "\n********** SUBMARINE **********\n";
			cout << "1. Up\n";
			cout << "2. Down\n";
			cout << "3. Left\n";
			cout << "4. Right";
			// bs.log update
			bsLog << "\n********** SUBMARINE **********\n";
			bsLog << "1. Up\n";
			bsLog << "2. Down\n";
			bsLog << "3. Left\n";
			bsLog << "4. Right";
			while (dir > 4 || dir < 1) {
				cout << "\nPlease Enter the Direction: ";
				bsLog << "\nPlease Enter the Direction: ";
				cin >> dir;
				bsLog << dir;
				if (dir > 4 || dir < 1) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a row (0-9) and make sure input is valid
			while (row < 0 || row > 9) {
				cout << "\nEnter Row (0-9): ";
				bsLog << "\nEnter Row (0-9): ";
				cin >> row;
				bsLog << row;
				if (row < 0 || row > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a column (0-9) and make sure input is valid
			while (col < 0 || col > 9) {
				cout << "\nEnter Column (0-9): ";
				bsLog << "\nEnter Column (0-9): ";
				cin >> col;
				bsLog << col;
				if (col < 0 || col > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// For-loop to place the ships on the board and a switch statement to determine the direction
			for (int i = shipCount; i >= 0; i--) {
				switch (dir) {

				// Up
				case 1:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row--][col] = 's';
					break;

				// Down
				case 2:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row++][col] = 's';
					break;

				// Left
				case 3:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col--] = 's';
					break;

				// Right
				case 4:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col++] = 's';
					break;
				}
			}
			// Decrement shipCount and move onto next ship
			shipCount--;
		}
		// Destroyer
		else if (shipCount == 1) {
			// Prompt the user for a direction (up, down, left, right) and make sure input is valid
			cout << "\n********** DESTROYER **********\n";
			cout << "1. Up\n";
			cout << "2. Down\n";
			cout << "3. Left\n";
			cout << "4. Right";
			// bs.log update
			bsLog << "\n********** DESTROYER **********\n";
			bsLog << "1. Up\n";
			bsLog << "2. Down\n";
			bsLog << "3. Left\n";
			bsLog << "4. Right";
			while (dir > 4 || dir < 1) {
				cout << "\nPlease Enter the Direction: ";
				bsLog << "\nPlease Enter the Direction: ";
				cin >> dir;
				bsLog << dir;
				if (dir > 4 || dir < 1) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a row (0-9) and make sure input is valid
			while (row < 0 || row > 9) {
				cout << "\nEnter Row (0-9): ";
				bsLog << "\nEnter Row (0-9): ";
				cin >> row;
				bsLog << row;
				if (row < 0 || row > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// Prompt the user for a column (0-9) and make sure input is valid
			while (col < 0 || col > 9) {
				cout << "\nEnter Column (0-9): ";
				bsLog << "\nEnter Column (0-9): ";
				cin >> col;
				bsLog << col;
				if (col < 0 || col > 9) {
					cout << "\nInvalid Input!";
					bsLog << "\nInvalid Input!";
				}
			}
			// For-loop to place the ships on the board and a switch statement to determine the direction
			for (int i = shipCount; i >= 0; i--) {
				switch (dir) {

				// Up
				case 1:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[++row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row--][col] = 'd';
					break;

				// Down
				case 2:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[--row][col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row++][col] = 'd';
					break;

				// Left
				case 3:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col < 0) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][++col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col--] = 'd';
					break;

				// Right
				case 4:
					// If the ship goes out of bounds or there is already an existent ship, reset to placement
					if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col > 9) {
						if (shipCount == 1 || shipCount == 2) {
							for (int j = i; j <= shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						else {
							for (int j = i; j < shipCount; j++) {
								p1Board[row][--col] = '-';
							}
						}
						cout << "\nPlacement invalid, please try again.";
						bsLog << "\nPlacement invalid, please try again.";
						goto reset;
					}
					// Place the ship on the board
					p1Board[row][col++] = 'd';
					break;
				}
			}
			// Decrement shipCount and move onto next ship
			shipCount--;
		}
		// Display the board and pause for user to move onto next step
		display_single_board(p1Board);
		system("pause");
		system("CLS");
		// If shipCount is 0, exit the loop / function
		if (shipCount == 0) {
			choosing = false;
		}
	}
}

void init_board_read(ofstream& bsLog, string file_name, char p1Board[BOARD_SIZE][BOARD_SIZE]) {

	// Initialize variables
	ifstream file;
	int shipType;
	char shipChar;
	int shipSize;
	int shipCounter = 5;
	int row;
	int col;
	bool choosing = true;

	// Open the file
	file.open(file_name);
	// If the file is open, go through and check the file for input
	if (file.is_open()) {
		// Ship Placement
		while (choosing) {
			// Get the first character of the line which is the ship type
			file >> shipType;
			// Run through the different ship cases, and set the size/character for each one
			switch (shipType) {
				// Destroyer
			case 1:
				shipSize = 2;
				shipChar = 'd';
				break;
				// Submarine
			case 2:
				shipSize = 3;
				shipChar = 's';
				break;
				// Cruiser
			case 3:
				shipSize = 3;
				shipChar = 'r';
				break;
				// Battleship
			case 4:
				shipSize = 4;
				shipChar = 'b';
				break;
				// Carrier
			case 5:
				shipSize = 5;
				shipChar = 'c';
				break;
			}
			// For-loop to place the ships on the board
			for (int i = 0; i < shipSize; i++) {
				// Read the row and column and store the information
				file >> row;
				file >> col;
				// Check that the file has correct placement rows and columns and exit the program if it doesn't
				if (row < 0 || row > 9 || col < 0 || col > 9) {
					cout << "\nInvalid Input! Check your file for misplacement and try again.";
					bsLog << "\nInvalid Input! Check your file for misplacement and try again.";
					exit(0);
				}
				// Check that no ships are crashing into each other
				else if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd') {
					cout << "\nShip Collision! Check your file for collisions and try again.";
					bsLog << "\nShip Collision! Check your file for collisions and try again.";
					exit(1);
				}
				// Place the ships on the board
				else {
					p1Board[row][col] = shipChar;
				}
			}
			// Decrement the shipCounter and exit the loop when it is at zero (all ships placed)
			shipCounter--;
			if (shipCounter == 0) {
				choosing = false;
			}
		}
	}
}

void init_board_random(char p1Board[BOARD_SIZE][BOARD_SIZE]) {

	// Initialize variables
	int row;
	int col;
	int dir;
	int shipCount = 5;
	int shipSize;
	char shipChar;
	bool choosing = true;

	// Ship Placement
	while (choosing) {
		// Resets the random generator if cases are invalid
	randreset:
		// Randomize the direction (1-4), row (0-9), and column (0-9)
		dir = rand() % 4 + 1;
		row = rand() % 10;
		col = rand() % 10;

		// Set the size of the ship
		if (shipCount == 1 || shipCount == 2) {
			shipSize = shipCount + 1;
		}
		else {
			shipSize = shipCount;
		}

		// Assign shipChar based on shipCount value to a letter
		if (shipCount == 5) {
			shipChar = 'c';
		}
		else if (shipCount == 4) {
			shipChar = 'b';
		}
		else if (shipCount == 3) {
			shipChar = 'r';
		}
		else if (shipCount == 2) {
			shipChar = 's';
		}
		else if (shipCount == 1) {
			shipChar = 'd';
		}
		// If shipCount = 0, exit loop / function
		else {
			choosing = false;
		}

		// For-loop to place the ships on the board
		for (shipSize; shipSize > 0; shipSize--) {
			switch (dir) {

			// Up
			case 1:
				// If the ship goes out of bounds or there is already an existent ship, reset to generation
				if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row < 0) {
					if (shipCount == 1 || shipCount == 2) {
						for (int i = shipSize; i <= shipCount; i++) {
							p1Board[++row][col] = '-';
						}
					}
					else {
						for (int i = shipSize; i < shipCount; i++) {
							p1Board[++row][col] = '-';
						}
					}
					goto randreset;
				}
				// Place the ship on the board
				p1Board[row--][col] = shipChar;
				break;

			// Down
			case 2:
				// If the ship goes out of bounds or there is already an existent ship, reset to generation
				if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || row > 9) {
					if (shipCount == 1 || shipCount == 2) {
						for (int i = shipSize; i <= shipCount; i++) {
							p1Board[--row][col] = '-';
						}
					}
					else {
						for (int i = shipSize; i < shipCount; i++) {
							p1Board[--row][col] = '-';
						}
					}
					goto randreset;
				}
				// Place the ship on the board
				p1Board[row++][col] = shipChar;
				break;

			// Left
			case 3:
				// If the ship goes out of bounds or there is already an existent ship, reset to generation
				if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col < 0) {
					if (shipCount == 1 || shipCount == 2) {
						for (int i = shipSize; i <= shipCount; i++) {
							p1Board[row][++col] = '-';
						}
					}
					else {
						for (int i = shipSize; i < shipCount; i++) {
							p1Board[row][++col] = '-';
						}
					}
					goto randreset;
				}
				// Place the ship on the board
				p1Board[row][col--] = shipChar;
				break;

			// Right
			case 4:
				// If the ship goes out of bounds or there is already an existent ship, reset to generation
				if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd' || col > 9) {
					if (shipCount == 1 || shipCount == 2) {
						for (int i = shipSize; i <= shipCount; i++) {
							p1Board[row][--col] = '-';
						}
					}
					else {
						for (int i = shipSize; i < shipCount; i++) {
							p1Board[row][--col] = '-';
						}
					}
					goto randreset;
				}
				// Place the ship on the board
				p1Board[row][col++] = shipChar;
				break;
			}
		}
		// Decrement shipCount
		shipCount--;
	}
}

void p1update(ofstream& bsLog, char p2Board[BOARD_SIZE][BOARD_SIZE], char p2HiddenBoard[BOARD_SIZE][BOARD_SIZE], int row, int col, Player& p1, Player& p2) {

	// Increase the total shots for player 1
	p1.total++;

	// If the shot was a hit
	if (p2Board[row][col] == 'c' || p2Board[row][col] == 'b' || p2Board[row][col] == 'r' || p2Board[row][col] == 's' || p2Board[row][col] == 'd') {
		// Print the hit and increase the total hits for player 1
		cout << "\nPLAYER1 - You hit a ship! [ " << row << " , " << col << " ]\n";
		bsLog << "\nPLAYER1 - You hit a ship! [ " << row << " , " << col << " ]\n";
		p1.hit++;

		// Decrement the array index for that ship
		// Carrier
		if (p2Board[row][col] == 'c') {
			p2.ships[4] -= 1;
		}
		// Battleship
		else if (p2Board[row][col] == 'b') {
			p2.ships[3] -= 1;
		}
		// Cruiser
		else if (p2Board[row][col] == 'r') {
			p2.ships[2] -= 1;
		}
		// Submarine
		else if (p2Board[row][col] == 's') {
			p2.ships[1] -= 1;
		}
		// Destroyer
		else if (p2Board[row][col] == 'd') {
			p2.ships[0] -= 1;
		}

		// Update player 2 boards to show a hit
		p2Board[row][col] = '*';
		p2HiddenBoard[row][col] = '*';
	}
	else if (p2Board[row][col] == '-') {
		// Print the hit and increase the total hits for player 1
		cout << "\nPLAYER1 - You missed! [ " << row << " , " << col << " ]\n";
		bsLog << "\nPLAYER1 - You missed! [ " << row << " , " << col << " ]\n";
		p1.miss++;

		// Update player 2 boards to show a miss
		p2Board[row][col] = 'm';
		p2HiddenBoard[row][col] = 'm';
	}

	// Check if any ships sunk
	check_if_sunk(bsLog, p2);

	// Pause for user to press a key and clear the screen
	system("pause");
	system("CLS");
}

void p2update(ofstream& bsLog, char p1Board[BOARD_SIZE][BOARD_SIZE], int row, int col, Player& p1, Player& p2) {

	// Increase the total shots for player 1
	p2.total++;

	// If the shot was a hit
	if (p1Board[row][col] == 'c' || p1Board[row][col] == 'b' || p1Board[row][col] == 'r' || p1Board[row][col] == 's' || p1Board[row][col] == 'd') {
		// Print the hit and increase the total hits for player 1
		cout << "\nPLAYER2 - Hit a ship! [ " << row << " , " << col << " ]\n";
		bsLog << "\nPLAYER2 - Hit a ship! [ " << row << " , " << col << " ]\n";
		p2.hit++;

		// Decrement the array index for that ship
		// Carrier
		if (p1Board[row][col] == 'c') {
			p1.ships[4] -= 1;
		}
		// Battleship
		else if (p1Board[row][col] == 'b') {
			p1.ships[3] -= 1;
		}
		// Cruiser
		else if (p1Board[row][col] == 'r') {
			p1.ships[2] -= 1;
		}
		// Submarine
		else if (p1Board[row][col] == 's') {
			p1.ships[1] -= 1;
		}
		// Destroyer
		else if (p1Board[row][col] == 'd') {
			p1.ships[0] -= 1;
		}

		// Update player 2 boards to show a hit
		p1Board[row][col] = '*';
	}
	else if (p1Board[row][col] == '-') {
		// Print the hit and increase the total hits for player 1
		cout << "\nPLAYER2 - Missed! [ " << row << " , " << col << " ]\n";
		bsLog << "\nPLAYER2 - Missed! [ " << row << " , " << col << " ]\n";
		p2.miss++;

		// Update player 2 boards to show a miss
		p1Board[row][col] = 'm';
	}

	// Check if any ships sunk
	check_if_sunk(bsLog, p1);

	// Pause for user to press a key and clear the screen
	system("pause");
	system("CLS");
}

bool is_winner(char gameBoard[BOARD_SIZE][BOARD_SIZE]) {

	bool winner = false;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] == 'c' || gameBoard[i][j] == 'b' || gameBoard[i][j] == 'r' || gameBoard[i][j] == 's' || gameBoard[i][j] == 'd') {
				winner = true;
			}
		}
	}

	return winner;
}

void check_if_sunk(ofstream& bsLog, Player& player) {

	if (player.playerNum == 1) {

		if (player.ships[0] == 0) {
			cout << "\nPlayer 1 Destroyer has sunk!\n";
			bsLog << "\nPlayer 1 Destroyer has sunk!\n";
			player.ships[0] = -1;
		}
		else if (player.ships[1] == 0) {
			cout << "\n Player 1 Submarine has sunk!\n";
			bsLog << "\n Player 1 Submarine has sunk!\n";
			player.ships[1] = -1;
		}
		else if (player.ships[2] == 0) {
			cout << "\n Player 1 Cruiser has sunk!\n";
			bsLog << "\n Player 1 Cruiser has sunk!\n";
			player.ships[2] = -1;
		}
		else if (player.ships[3] == 0) {
			cout << "\n Player 1 Battleship has sunk!\n";
			bsLog << "\n Player 1 Battleship has sunk!\n";
			player.ships[3] = -1;
		}
		else if (player.ships[4] == 0) {
			cout << "\n Player 1 Carrier has sunk!\n";
			bsLog << "\n Player 1 Carrier has sunk!\n";
			player.ships[4] = -1;
		}
	}
	else if (player.playerNum == 2) {

		if (player.ships[0] == 0) {
			cout << "\nPlayer 2 Destroyer has sunk!\n";
			bsLog << "\nPlayer 2 Destroyer has sunk!\n";
			player.ships[0] = -1;
		}
		else if (player.ships[1] == 0) {
			cout << "\n Player 2 Submarine has sunk!\n";
			bsLog << "\n Player 2 Submarine has sunk!\n";
			player.ships[1] = -1;
		}
		else if (player.ships[2] == 0) {
			cout << "\n Player 2 Cruiser has sunk!\n";
			bsLog << "\n Player 2 Cruiser has sunk!\n";
			player.ships[2] = -1;
		}
		else if (player.ships[3] == 0) {
			cout << "\n Player 2 Battleship has sunk!\n";
			bsLog << "\n Player 2 Battleship has sunk!\n";
			player.ships[3] = -1;
		}
		else if (player.ships[4] == 0) {
			cout << "\n Player 2 Carrier has sunk!\n";
			bsLog << "\n Player 2 Carrier has sunk!\n";
			player.ships[4] = -1;
		}
	}
}

void display_single_board(char gameBoard[BOARD_SIZE][BOARD_SIZE]) {

	cout << "    0 1 2 3 4 5 6 7 8 9\n";
	cout << "    ~~~~~~~~~~~~~~~~~~~\n";

	for (int i = 0; i < BOARD_SIZE; i++) {

		cout << i << " | ";

		for (int j = 0; j < BOARD_SIZE; j++) {
			cout << gameBoard[i][j] << " ";
		}
		cout << endl;
	}
}

void display_both_boards(char p1Board[BOARD_SIZE][BOARD_SIZE], char p2Board[BOARD_SIZE][BOARD_SIZE]) {

	// Player 1
	cout << "    Player 1           \n";
	cout << "    0 1 2 3 4 5 6 7 8 9\n";
	cout << "    ~~~~~~~~~~~~~~~~~~~\n";

	for (int i = 0; i < BOARD_SIZE; i++) {

		cout << i << " | ";

		for (int j = 0; j < BOARD_SIZE; j++) {
			cout << p1Board[i][j] << " ";
		}
		cout << endl;
	}

	// Player 2
	cout << "\n    Player 2           \n";
	cout << "    0 1 2 3 4 5 6 7 8 9\n";
	cout << "    ~~~~~~~~~~~~~~~~~~~\n";

	for (int i = 0; i < BOARD_SIZE; i++) {

		cout << i << " | ";

		for (int j = 0; j < BOARD_SIZE; j++) {
			cout << p2Board[i][j] << " ";
		}
		cout << endl;
	}
}

void output_stats(ofstream& bsLog, Player& p1, Player& p2) {

	// Initialize variables
	p1.hitPercent = (p1.hit / p1.total) * 100;
	p2.hitPercent = (p2.hit / p2.total) * 100;
	p1.missPercent = (p1.miss / p1.total) * 100;
	p2.missPercent = (p2.miss / p1.total) * 100;

	system("CLS");

	// Display end game statistics
	cout << "\n********** END GAME STATISTICS **********";
	cout << "\n\n-----------------------------------------";
	cout << "\n               PLAYER 1                  ";
	cout << "\n-----------------------------------------";
	cout << "\nTotal Shots: " << p1.total;
	cout << "\nTotal Hits : " << p1.hit;
	cout << "\nTotal Misses: " << p1.miss;
	cout << "\n\nHit Percent: " << setprecision(3) << p1.hitPercent << "%";
	cout << "\nMiss Percent: " << setprecision(3) << p1.missPercent << "%";
	cout << "\n-----------------------------------------";
	cout << "\n\n-----------------------------------------";
	cout << "\n               PLAYER 2                  ";
	cout << "\n-----------------------------------------";
	cout << "\nTotal Shots: " << p2.total;
	cout << "\nTotal Hits : " << p2.hit;
	cout << "\nTotal Misses: " << p2.miss;
	cout << "\n\nHit Percent: " << setprecision(3) << p2.hitPercent << "%";
	cout << "\nMiss Percent: " << setprecision(3) << p2.missPercent << "%";
	cout << "\n-----------------------------------------";

	// Store end game statistics in bs.log
	bsLog << "\n********** END GAME STATISTICS **********";
	bsLog << "\n\n-----------------------------------------";
	bsLog << "\n               PLAYER 1                  ";
	bsLog << "\n-----------------------------------------";
	bsLog << "\nTotal Shots: " << p1.total;
	bsLog << "\nTotal Hits : " << p1.hit;
	bsLog << "\nTotal Misses: " << p1.miss;
	bsLog << "\n\nHit Percent: " << setprecision(3) << p1.hitPercent << "%";
	bsLog << "\nMiss Percent: " << setprecision(3) << p1.missPercent << "%";
	bsLog << "\n-----------------------------------------";
	bsLog << "\n\n-----------------------------------------";
	bsLog << "\n               PLAYER 2                  ";
	bsLog << "\n-----------------------------------------";
	bsLog << "\nTotal Shots: " << p2.total;
	bsLog << "\nTotal Hits : " << p2.hit;
	bsLog << "\nTotal Misses: " << p2.miss;
	bsLog << "\n\nHit Percent: " << setprecision(3) << p2.hitPercent << "%";
	bsLog << "\nMiss Percent: " << setprecision(3) << p2.missPercent << "%";
	bsLog << "\n-----------------------------------------";
}
