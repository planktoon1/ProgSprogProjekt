#include "pch.h"
#include <iostream>
#include <stack>
#include <string>

using namespace std;
enum BoardOptions { player1, player2, empty};
class Player {};
class Board {
	private:
		BoardOptions gameBoard[7][6];
		BoardOptions currentPlayer = BoardOptions::player1;

	public:
	Board() {
		for (int row = 0; row < 6; row++) {
			for (int col = 0; col < 7; col++) {
				this->gameBoard[col][row] = BoardOptions::empty;
			}
		}
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

	void reset(bool list[], bool &won) {
		if (list[1] && list[2] && list[3]) won = true;
		else {
			list[1] = false;
			list[2] = false;
			list[3] = false;
		}
	}

	void checkConnectedFour(int col, int row) {

		bool won;
		BoardOptions currentPlayer = this->gameBoard[col][row];
		bool fourInARow[4]{true};
		// Check horizontal right
		for (int i = 1; i < 4; i++) {
			if (col + i <= 6 && this->gameBoard[col + i][row] == currentPlayer) {
				fourInARow[i] = true;
			} else break;
		}
		reset(fourInARow, won);

		// Check horizontal left
		if (!won) {
			for (int i = 1; i > 4; i++) {
				cout << currentPlayer << endl;
				if (col - i >= 0 && this->gameBoard[col - i][row] == currentPlayer) {
					fourInARow[i] = true;
				}
				else break;
			}
			reset(fourInARow, won);
		}

		// Check vertical down
		if (!won) {
			for (int i = 1; i > 4; i++) {
				cout << currentPlayer << endl;
				if (row + i <= 5 && this->gameBoard[col][row + i] == currentPlayer) {
					fourInARow[i] = true;
				}
				else break;
			}
			reset(fourInARow, won);
		}

		// Check diagonal down right
		if (!won) {
			for (int i = 1; i > 4; i++) {
				cout << currentPlayer << endl;
				if (row + i <= 5 && col + i <= 6 && this->gameBoard[col +i][row + i] == currentPlayer) {
					fourInARow[i] = true;
				}
				else break;
			}
			reset(fourInARow, won);
		}


		// Check diagonal down left

		if (won) {
			string cp = (currentPlayer == BoardOptions::player1) ? "Player 1" : "Player 2";
			cout << cp << " WON!!!" << endl;
		}
	}

	void drawBoard() {
		cout << "\x1B[2J\x1B[H"; // Clear the console
		cout << " \n  Connected four " << endl;
		string currentPlayer = (this->currentPlayer == BoardOptions::player1) ? "Player 1" : "Player 2";
		cout << "  Current player: " << currentPlayer << endl;

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
};

int main()
{
	Board b1;
	b1.drawBoard();
	cout << " \n  Welcome to connected four. " << endl;
	
	while (true) {
		int input;
		cout << "What column do you wanna insert into? : ";
		cin >> input;
		b1.insertPiece(input);
	}

	return 0;
}
