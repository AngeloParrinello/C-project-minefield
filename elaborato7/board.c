

#include "game.h" 
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
* Fills the rows*cols board with num_mines random mines
* leaving free the neighborhood of position i,j
*/
void random_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j, unsigned int num_mines)
{
	static unsigned int ncolonne;
	static unsigned int nrighe;
	static  unsigned int mines;
	mines = num_mines;
	srand(time(NULL));
	while (mines > 0)
	{
		//metto le mine e man mano decremento il contatore
		unsigned int x = rand() % rows;
		unsigned int k = rand() % cols;
		if ((x != i && k != j) && (x != i && k != j - 1) && (x != i && k != j + 1) && (x != i - 1 && k != j - 1) && (x != i - 1 && k != j) && (x != i - 1 && k != j + 1) && (x != i + 1 && k != j - 1) && (x != i + 1 && k != j) && (x != i + 1 && k != j + 1) && (board[x][k] != UNKN_MINE))
		{
			board[x][k] = UNKN_MINE;
			mines--;


		}


	} //metto nelle restanti celle che son libere
	for (nrighe = 0; nrighe < rows; nrighe++) {
		for (ncolonne = 0; ncolonne < cols; ncolonne++) {
			if (board[nrighe][ncolonne] != UNKN_MINE) board[nrighe][ncolonne] = UNKN_FREE;
		}
	}
}

/*
* Flags/unflags the i,j position in the board. Returns
* - -1 if the position was flagged. Removes the flag
* -  0 if the position is already displyed
* -  1 if the position is not flagged and not already
*    displayed. Puts a flag in position i,j.
*/
int flag_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j)
{
	if (board[i][j] == FLAG_FREE)
	{
		board[i][j] = UNKN_FREE;
		return -1;

	}
	if (board[i][j] == FLAG_MINE)
	{
		board[i][j] = UNKN_MINE;
		return -1;

	}
	if (board[i][j] == UNKN_FREE)
	{
		board[i][j] = FLAG_FREE;
		return 1;
	}
	if (board[i][j] == UNKN_MINE)
	{
		board[i][j] = FLAG_MINE;
		return 1;
	}
	if (board[i][j] <= C8) return 0;

}

/*
* Displays position i,j in the board. Returns the number of
* displayed cells or -1 if i,j contains a mine.
*/
int display_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j)
{
	unsigned int i1, j1; 
	unsigned int i0 = i, j0 = j;

	unsigned int contatore = 0;
	unsigned int contatore1 = 1;

	//controllo se cè una mina
	if (board[i][j] == UNKN_MINE)
	{
		board[i][j] = MINE;
		return -1;

	}

	//se è flaggata o stata scoperta già ritorno 0
	if ((board[i][j] <= C8) || (board[i][j] == FLAG_MINE) || (board[i][j] == FLAG_FREE))
	{
		return 0;
	}

	//conto quante mine ho attorno a i,j
	else /*if (board[i][j] == UNKN_FREE)*/
	{

		if (i == 0) i0 = 1;
		if (j == 0) j0 = 1;

		for (i1 = i0 - 1; (i1 <= i + 1) && (i1 != rows); i1++) {
			for (j1 = j0 - 1; (j1 <= j + 1) && (j1 != cols); j1++) {
				if (board[i1][j1] == UNKN_MINE) contatore++;
				if (board[i1][j1] == FLAG_MINE) contatore++;
			}
		}



		//metto in i,j il numero di mine che lo circondano
		board[i][j] = contatore;

		//apro ricorsivamente il resto delle celle
		if (board[i][j] == 0) {



			for (i1 = i0 - 1; (i1 <= i + 1) && (i1 != rows); i1++) {
				for (j1 = j0 - 1; (j1 <= j + 1) && (j1 != cols); j1++) {
					contatore1 = contatore1 + display_board(board, rows, cols, i1, j1);
				}
			}

		}
		return contatore1;
	}



}


/*
* Expands all the free cells sourrounding  position i,j in
* the board. Returns the number of expanded cells or -1 if
* one contains a mine.
*/
int expand_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j)
{

	unsigned int i1, j1;
	unsigned int i0 = i, j0 = j;
	unsigned int contatore = 0;
	unsigned int contatore1 = 0;


	//conto quante flag ho attorno a i,j
	if (i == 0) i0 = 1;
	if (j == 0) j0 = 1;

	for (i1 = i0 - 1; (i1 <= i + 1) && (i1 != rows); i1++) {
		for (j1 = j0 - 1; (j1 <= j + 1) && (j1 != cols); j1++) {
				if (board[i1][j1] == FLAG_FREE) contatore1++;
				if (board[i1][j1] == FLAG_MINE) contatore1++;
			}
		}
	
	//se la posizione i,j non contiene attorno un numero di celle flaggate pari al numero mostrato in i,j
	if (board[i][j] > contatore1) return 0;

	if (board[i][j] <= contatore1)
	{

		for (i1 = i0 - 1; (i1 <= i + 1) && (i1 != rows); i1++) {
			for (j1 = j0 - 1; (j1 <= j + 1) && (j1 != cols); j1++) {
					contatore = contatore + display_board(board, rows, cols, i1, j1);
					if (board[i1][j1] == MINE)return -1; //controllo se c'è una mina
				}
			}
		return contatore;
		
	}
		
	



}

