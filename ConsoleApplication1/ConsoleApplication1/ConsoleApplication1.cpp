#include "pch.h"
#include <iostream>
#include <Regex>
#include <string>
#include <windows.h>

using namespace std;
enum BoardOptions { empty, player1, player2};
class Player {};
class Board {	
	private:
		BoardOptions gameBoard[7][6];
		BoardOptions currentPlayer = BoardOptions::player1;
		bool gameFinished = false;

		// helper method for the "checkConnectedFour" method
		void reset(bool list[], bool &gameFinished) {
			int counter = 0;
			for (int i = 0; i < 7; i++) {
				if (list[i] == true) {
					counter++;
					if (counter == 4) { gameFinished = true; return; }
				}
				else {
					counter = 0;
				}
			}
			list[0] = false;
			list[1] = false;
			list[2] = false;
			list[3] = false;
			list[4] = false;
			list[5] = false;
			list[6] = false;
		}

		void resetBoard() {
			for (int row = 0; row < 6; row++) {
				for (int col = 0; col < 7; col++) {
					this->gameBoard[col][row] = BoardOptions::empty;
				}
			}
			currentPlayer = BoardOptions::player1;
			gameFinished = false;
		}

		void insertPiece(int col) {
			for (int i = 5; i >= 0; i--) {
				if (this->gameBoard[col - 1][i] == BoardOptions::empty) {
					this->gameBoard[col - 1][i] = this->currentPlayer;
					this->currentPlayer = (this->currentPlayer == BoardOptions::player1) ? BoardOptions::player2 : BoardOptions::player1;
					this->drawBoard();
					this->checkConnectedFour(col - 1, i);
					return;
				}
			}
		
			cout << "Column is full, try again in another column" << endl;

		}

		void checkConnectedFour(int col, int row) {
			BoardOptions currentPlayer = this->gameBoard[col][row];
			bool fourInARow[7]{};

			// Check horizontal
			for (int i = -3; i <= 3; i++) {
				if (col + i <= 6 && col + i >= 0 && this->gameBoard[col + i][row] == currentPlayer) {
					fourInARow[i + 3] = true;
				}
			}
			reset(fourInARow, gameFinished);

			// Check vertical down
			if (!gameFinished) {
				for (int i = 0; i < 4; i++) {
					if (row + i <= 5 && this->gameBoard[col][row + i] == currentPlayer) {
						fourInARow[i] = true;
					}
					else break;
				}
				reset(fourInARow, gameFinished);
			}

			// Check diagonal right
			if (!gameFinished) {
				for (int i = -3; i <= 3; i++) {
					if (row + i <= 5 && row + i >= 0 && col + i <= 6 && col + i >= 0) {
						if (this->gameBoard[col + i][row + i] == currentPlayer) {
							fourInARow[i + 3] = true;
						}
					}
				}
				reset(fourInARow, gameFinished);
			}

			// Check diagonal left
			if (!gameFinished) {
				for (int i = -3; i <= 3; i++) {
					if (row + i <= 5 && row + i >= 0 && col - i <= 6 && col - i >= 0) {
						if (this->gameBoard[col - i][row + i] == currentPlayer) {
							fourInARow[i + 3] = true;
						}
					}
				}
				reset(fourInARow, gameFinished);
			}



			if (gameFinished == true) {
				string cp = (currentPlayer == BoardOptions::player1) ? "Player 1" : "Player 2";
				cout << cp << " WON!!!" << endl;
			}
		}

		void drawBoard() {
			cout << "\x1B[2J\x1B[H"; // Clear the console
			cout << " \n  Connected four " << endl;
			string currentPlayer = (this->currentPlayer == BoardOptions::player1) ? "Player 1 (X)" : "Player 2 (O)";
			cout << "  Current player: " << currentPlayer  << endl;

			string board = "  ___________________________";
			for (int row = 0; row < 6; row++) {
				board += "\n |";
				for (int col = 0; col < 7; col++) {
					switch (this->gameBoard[col][row]) {
					case BoardOptions::empty:
						board += "___|";
						break;
					case BoardOptions::player1:
						board += "_X_|";
						break;
					case BoardOptions::player2:
						board += "_O_|";
						break;
					}
				}
			}
			board += "\n   1   2   3   4   5   6   7";
			cout << board << endl;
		}

		int computerChoice() {
			BoardOptions options[7][4][7]{};
			
			// ----------------------------- Find options -----------------------------
			for (int col = 0; col < 7; col++){
				int row = 0;
				for (int i = 5; i >= 0; i--) {
					if (this->gameBoard[col][i] == BoardOptions::empty) {
						row = i;
						break;
					}
				}
				BoardOptions currentPlayer = this->gameBoard[col][row];


				// Check horizontal
				for (int i = -3; i <= 3; i++) {
					if (col + i <= 6 && col + i >= 0 ) {
						options[col][0][i + 3] = this->gameBoard[col+i][row];
					}
				}

				// Check vertical down
				for (int i = 0; i < 4; i++) {
					if (row + i <= 5) {
						options[col][1][i+3] = gameBoard[col][row + i];
					}
				}



				// Check diagonal right
				for (int i = -3; i <= 3; i++) {
					if (row + i <= 5 && row + i >= 0 && col + i <= 6 && col + i >= 0) {
						options[col][2][i + 3] = gameBoard[col + i][row + i];
					
					}
				}

				// Check diagonal left
				for (int i = -3; i <= 3; i++) {
					if (row + i <= 5 && row + i >= 0 && col - i <= 6 && col - i >= 0) {
						options[col][3][i + 3] = gameBoard[col - i][row + i];
					}
				}
			}

			// ----------------------------- Validate options -----------------------------
			srand(time(NULL));
			int choosenColumn = rand() % 7 + 1;
			int priority = 0; /*	0 = random 
									1 = 1 of your own
									2 = 2 of your own in a row  
									3 = 3 of the opponents in a row
									4 = 3 of your own in a row
							  */
			
			for (int dim1 = 0; dim1 < 7; dim1++) {
				if (this->gameBoard[dim1][0] != BoardOptions::empty)
					continue;
				for (int dim2 = 0; dim2 < 4; dim2++) {
					int humanCounter = 0;
					int computerCounter = 0; 
					for (int dim3 = 0; dim3 < 7; dim3++) {
						
						if (options[dim1][dim2][dim3] == BoardOptions::player2) { // computer
							computerCounter++;
						}
						else { //Done counting 
							if (computerCounter == 3) { // computer can win the game
								choosenColumn = dim1 +1;
								priority = 4;
							}  
							else if ((dim3 == 3 || dim3 == 6) && computerCounter == 2 && priority < 2) { // Computer can make 3 in a row
								choosenColumn = dim1 + 1;
								priority = 2;
							} else if ((dim3 == 3 || dim3 == 5) && computerCounter == 1 && priority < 1) { // Computer can make 2 in a row
								choosenColumn = dim1 + 1;
								priority = 1;
							}
							computerCounter = 0; // reset counter
						}

						if (options[dim1][dim2][dim3] == BoardOptions::player1) { // human 
							humanCounter++;
							if (humanCounter == 3 && priority < 4) { // Human can win, computer has to stop human
								choosenColumn = dim1 + 1;
								priority = 3;
							}
						}
						else {
							humanCounter = 0;
						}
					}
				}
			}

			return choosenColumn;
		}

		void computerTakeTurn() {
			if (gameFinished == true) return;

			Sleep(1000);
			int col = computerChoice();
			insertPiece(col);
			cout << "The computer inserted into column:" << col << endl;
		}

	public:
		Board() {
			resetBoard();
		}

		void play() {
			drawBoard();

			while (true) {
				string input;
				resetBoard();
				drawBoard();
				cout << " \n  Welcome to connected four. " << endl;
				cout << "  Play against computer or against another player? Type 'c' for computer / 'p' for player" << endl;
				cin >> input;
				if (input == "p") { // Game against other player
					while (true) {
						cout << "What column do you wanna insert into? (type 'r' to reset game): ";
						cin >> input;

						regex r("[1-7]");
						if (regex_match(input, r))
							insertPiece(stoi(input));
						else if (input == "r") {
							break;
						}
						else
							cout << "Please enter number from 1-7" << endl;
					}
				}
				else if (input == "c") { // Game against the computer
					while (true) {
						cout << "What column do you wanna insert into? (type 'r' to reset game): ";
						cin >> input;

						regex r("[1-7]");
						if (regex_match(input, r))
							insertPiece(stoi(input));
						else if (input == "r") {
							break;
						}
						else
							cout << "Please enter number from 1-7" << endl;
						if (!gameFinished)
							computerTakeTurn();
					}
				}
				else {
					cout << "please choose 'c' or 'p'" << endl;
				}
			}

			
		}
};

int main()
{
	Board b1;
	b1.play();
	return 0;
}
