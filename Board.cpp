/*
 * Board.cpp - Class board C++ source file
 * Handle everything related to the board in the game Caro
 * 
 * Date: April 2016
 * Rev: 1.1
 * Author: Team 8 - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
 * 
 * Revision History:
 *  - 1.0: First release
 *  - 1.1: fix function ConfigBoard
*/
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include "Board.h"
using namespace std;

/****************************Private Definitions******************************/
#define GOTONEXTPROCESS			CheckProcess>>=1
#define BACKTOCURPOS			CurCheckPos_R=Pos_R;CurCheckPos_C=Pos_C=

/*
	The related position that is next to the current position
	
	  0x80	|		0x20	  |	0x08
	--------+-----------------+------
	  0x01	|CurrentPosition  |	0x02
	--------+-----------------+------
	   0x04 |		0x10	  |	0x40
*/
#define NEXTPOS_UL			0x80
#define NEXTPOS_DR			0x40
#define NEXTPOS_U			0x20
#define NEXTPOS_D			0x10
#define NEXTPOS_UR			0x08
#define NEXTPOS_DL			0x04
#define NEXTPOS_R			0x02
#define NEXTPOS_L			0x01

/*********************************Variables***********************************/
const char Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";	//Alphabet table
static unsigned int uiCount;

/****************************Function definitions*****************************/

/******************************************************************************
 * Draw the caro board to the screen, including the players markers
 * The board example:
 * 			**A*B*C*D...*
 * 			A     O X...*
 *			B   X   O...*
 *                ...
 *          *************
 *
 * Class: CaroBoard
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void CaroBoard::DrawBoard(void)
{
	system("cls");
	//Draw first line contain name of the columns
	cout << "*";
	for (int i = 0; i < this->Width; i++)
		cout << "*" << Alphabet[i];
	cout << "*" << endl;
	//Draw the board
	for (int i = 0; i < this->Height; i++)
	{
		cout << Alphabet[i];
		for (int j = 0; j < this->Width; j++)
			cout << " " << this->board[i][j];
		cout << "*" << endl;
	}
	//Draw last line
	for (int i = 0; i < this->Width; i++)
		cout << "**";
}

/******************************************************************************
 * Updating the board.
 * This function update the board every time a player marked in. So this
 * function should be called after a player have chose their position to marked
 * and the position is checked.
 *
 * Class: CaroBoard
 *
 * Parameter:
 * 	Pos_R: Row postion player just marked
 * 	Pos_C: Column position player just marked
 * 	M	 : Player's marker type
 *
 * Return: none
 *****************************************************************************/
void CaroBoard::UpdateBoard(int Pos_R, int Pos_C, Marker M)
{
	this->board[Pos_R][Pos_C] = M;
	uiCount++;
}

/******************************************************************************
 * Configure the board.
 * This function init the board, configures some neccessary parameters of the board
 * such as: width, height and the number of continous markers to win the game.
 * 
 * Note: The acceptable values for those parameters are
 * 			0 < Width <= 26;	0 < Height <= 25;		3 <= N <= 5
 * 
 * Class: CaroBoard
 * 
 * Parameter: none
 *
 * Return:
 *  true : config sucess
 *  false: config fail
 *****************************************************************************/
bool CaroBoard::ConfigBoard(void)
{
	system("cls");

	//Input width, height, N of the board
	cout << "Please setup board's width, height and the number of point to win" << endl;
config:
#ifdef _USE_EXTERNAL_FILE_
	fstream File;
	string strHeader;
	File.open("conf.txt");
	if (File.is_open())
	{
		File.getline(strHeader, 100);
		File.seekg(strHeader.length() + 1);
		File >> this->Width >> this->Height >> this->N;
	}
	else
	{
		cout << "File not found!";
		return false;
	}
#else
	//Reset variable
	this->Width = 0; this->Height = 0; this->N = 0;uiCount = 0;

	//The program shall go nowhere if the user input not correct
	while ((this->Width > BOARD_MAXWIDTH) || (this->Width < BOARD_MINN))
	{
		cout << "Width = ";

		cin.clear();
		cin.ignore();
		cin >> this->Width;
		if (cin.fail())
			cout << "Please enter a number between " << BOARD_MINN << " and " << BOARD_MAXWIDTH << " !" << endl;
	}
	while ((this->Height > BOARD_MAXHEIGHT) || (this->Height < BOARD_MINN))
	{
		cout << "Height = ";

		cin.clear();
		cin.ignore();
		cin >> this->Height;
		if (cin.fail())
			cout << "Please enter a number between " << BOARD_MINN << " and " << BOARD_MAXHEIGHT << " !" << endl;
	}
	while ((this->N > BOARD_MAXN) || (this->N < BOARD_MINN))
	{
		cout << "N = ";

		cin.clear();
		cin.ignore();
		cin >> this->N;
		if (cin.fail())
			cout << "Please enter a number between " << BOARD_MINN << " and " << BOARD_MAXN << endl;
	}
#endif
	
	//Additional checking
	if ((this->Width < this->N) || (this->Height < this->N))
	{
		cout << "With your configuration. You will never be able to win the game." << \
			endl << "Try another configuration";
#ifdef _USE_EXTERNAL_FILE_
		cout << " and restart the game" << endl;
		return false;
#else
		cout << endl;
		goto config;
#endif
		
	}
	
	this->board = new Marker*[Height];	//Make a new board
	for (int i = 0; i < this->Height; i++)
		this->board[i] = new Marker[Width];
	for (int i = 0, j = 0; (i < this->Height); (j < this->Width) ? (++j) : (i++, j = 0))	//Initialize values for the board
		this->board[i][j] = ' ';		//By default, all values are empty. But for printing issue, the values is set to ' '
		
	return true;
}

/******************************************************************************
 * Check the condition to win the game.
 * This function check the winning condition at the just-marked position. The
 * main idea is to find the same type marker at the position near the current
 * one in specific direction. There are 4 directions to look for: vertial,
 * horizontal, diogonal left, diogonal right.
 *
 * Class: CaroBoard
 *
 * Parameter:
 * 	Pos_R  : Row postion player just marked
 * 	Pos_C  : Column position player just marked
 * 	CurPlyM: Current player's marker
 *
 * Return: the result whether the game has ended or not
 * 	true : current player win the game
 *	false: the game continue
 *****************************************************************************/
bool CaroBoard::CheckBoard(int Pos_R, int Pos_C, Marker CurPlyM)
{
	uint8_t CheckProcess = NEXTPOS_UL;
	int Combo = 1;
	int CurCheckPos_R = Pos_R, CurCheckPos_C = Pos_C;
	
	//Start process, begin with the up left direction
	while (CheckProcess != 0)
	{
		switch (CheckProcess)
		{
			case NEXTPOS_UL:
				if (((CurCheckPos_R - 1) >= 0) && ((CurCheckPos_C - 1) >= 0))
					if (this->board[CurCheckPos_R - 1][CurCheckPos_C - 1] == CurPlyM)
					{
						Combo++;
						CurCheckPos_R--;		//Keep checking the next up left position
						CurCheckPos_C--;
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						GOTONEXTPROCESS;
					}
				else
				{
					BACKTOCURPOS;
					GOTONEXTPROCESS;
				}
			break;
				
			case NEXTPOS_U:
				if ((CurCheckPos_R - 1) >= 0)
					if (this->board[CurCheckPos_R - 1][CurCheckPos_C] == CurPlyM)
					{
						Combo++;
						CurCheckPos_R--;		//Keep checking the next up position
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						GOTONEXTPROCESS;
					}
				else
				{
					BACKTOCURPOS;
					GOTONEXTPROCESS;
				}
			break;
			
			case NEXTPOS_UR:
				if (((CurCheckPos_R - 1) >= 0) && ((CurCheckPos_C + 1) < this->Width))
					if (this->board[CurCheckPos_R - 1][CurCheckPos_C + 1] == CurPlyM)
					{
						Combo++;
						CurCheckPos_R--;		//Keep checking the next up right position
						CurCheckPos_C++;
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						GOTONEXTPROCESS;
					}
				else
				{
					BACKTOCURPOS;
					GOTONEXTPROCESS;
				}
			break;
			
			case NEXTPOS_R:
				if ((CurCheckPos_C + 1) < this->Width)
					if (this->board[CurCheckPos_R][CurCheckPos_C + 1] == CurPlyM)
					{
						Combo++;
						CurCheckPos_C++;		//Keep checking the next right position
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						GOTONEXTPROCESS;
					}
				else
				{
					BACKTOCURPOS;
					GOTONEXTPROCESS;
				}
			break;
			
			case NEXTPOS_DR:
				if (((CurCheckPos_R + 1) < this->Height) && ((CurCheckPos_C + 1) < this->Width))
					if (this->board[CurCheckPos_R + 1][CurCheckPos_C + 1] == CurPlyM)
					{
						Combo++;
						CurCheckPos_R++;		//Keep checking the next down right position
						CurCheckPos_C++;
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						Combo = 1;		//Reset the combo value when change the direction
						GOTONEXTPROCESS;
					}
				else
				{
					BACKTOCURPOS;
					Combo = 1;
					GOTONEXTPROCESS;
				}		
				break;
			
			case NEXTPOS_D:
				if ((CurCheckPos_R + 1) < this->Height)
					if (this->board[CurCheckPos_R + 1][CurCheckPos_C] == CurPlyM)
					{
						Combo++;
						CurCheckPos_R++;		//Keep checking the next down position
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						Combo = 1;		//Reset the combo value when change the direction
						GOTONEXTPROCESS;
					}
				else
					{
						BACKTOCURPOS;
						Combo = 1;		//Reset the combo value when change the direction
						GOTONEXTPROCESS;
					}
			break;
			
			case NEXTPOS_DL:
				if (((CurCheckPos_R + 1) < this->Height) && ((CurCheckPos_C - 1) >= 0))
					if (this->board[CurCheckPos_R + 1][CurCheckPos_C - 1] == CurPlyM)
					{
						Combo++;
						CurCheckPos_R++;		//Keep checking the next down left position
						CurCheckPos_C--;
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						Combo = 1;		//Reset the combo value when change the direction
						GOTONEXTPROCESS;
					}
				else
					{
						BACKTOCURPOS;
						Combo = 1;		//Reset the combo value when change the direction
						GOTONEXTPROCESS;
					}
			break;
			
			case NEXTPOS_L:
				if ((CurCheckPos_C - 1) >= 0)
					if (this->board[CurCheckPos_R][CurCheckPos_C - 1] == CurPlyM)
					{
						Combo++;
						CurCheckPos_C--;		//Keep checking the next left position
					}
					else		//Back to current position and the process continue
					{
						BACKTOCURPOS;
						GOTONEXTPROCESS;
					}
				else
					GOTONEXTPROCESS;
			break;
		}
		
		if (Combo == this->N)
			return true;
	}
	
	return false;
}

/******************************************************************************
 * Check if the position have filled or not. This function make sure every
 * position only be marked 1 time in the game.
 * 
 * Class: CaroBoard
 *
 * Parameter:
 *	Pos_R: the row position to check
 *	Pos_C: the column position to check
 *
 * Return
 *	true: the position hasn't filled yet
 *	false: the position has been filled
 *****************************************************************************/
bool CaroBoard::CheckMarkPos(int Pos_R, int Pos_C)
{
	if (this->board[Pos_R][Pos_C] != ' ') return false;
	return true;
}

/******************************************************************************
 * Check if the board has already filled fully yet
 *
 * Parameter: none
 *
 * Return:
 *  true : The board have fully filled
 *  false: hasn't fully filled yet
 *****************************************************************************/
bool CaroBoard::CheckFull(void)
{
	if (uiCount == (this->Width * this->Height))
		return true;
	return false;
}

/******************************************************************************
 * Destroy the board.
 * When the game end or player want to exit the game, the board will no longer
 * exist
 *
 * Class: CaroBoard
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void CaroBoard::DestroyBoard(void)
{
	for (int i = 0; i < this->Height;i++)
		delete [] this->board[i];
	delete [] this->board;
}

/******************************************************************************
 * Get the width and height from outside the class.
 *
 * Class: CaroBoard
 *
 * Parameter:
 *	*Width : store the Width of the board
 *	*Height: sore the height of the board
 *
 * Return: none
 *****************************************************************************/
void CaroBoard::GetParameters(int *Width, int *Height)
{
	*Width = this->Width;
	*Height = this->Height;
}

/* End of Board.cpp */
