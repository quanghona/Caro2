/*
 * Board.cpp - Class board C++ source file
 * Handle everything related to the board in the game Caro
 * 
 * Date: April 2016
 * Rev: 1.3
 * Author: Team 8
 * Group: TNMT - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
 * 
 * Revision History:
 *  - 1.0: First release
 *  - 1.1: updadte function ConfigBoard(), bring Alphabet table to Board.h, fix 
 * UpdateBoard() and CheckBoard(), add ConvertChartoNum()
 * 	- 1.2: add ConfigBoard(int, int, int), update ConfigBoard(), update UpdateBoard()
 *  - 1.3: Color marker available
*/
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include "Board.h"
#ifdef COLOR_MARKER
#include <Windows.h>
#endif
using namespace std;

/****************************Private Definitions******************************/
#define GOTONEXTPROCESS			CheckProcess>>=1
#define BACKTOCURPOS			CurCheckPos_R=Pos_R;CurCheckPos_C=Pos_C
#ifdef _USE_EXTERNAL_FILE_
	#define READFILEDATAFAIL	File.close();return false
#endif

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
#define COMPLETE			0x00

/*********************************Variables***********************************/
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
	//Draw first line contain name of the columns
#ifdef _EXPORT_TO_FILE_
	ofstream ExportFile("gameTrace.txt", ios::ate);	//Writing at the end of the file
	ExportFile << "*";
#endif
	cout << "*";
	
	
	for (int i = 0; i < this->Width; i++)
	{
		cout << "*" << Alphabet[i];
#ifdef _EXPORT_TO_FILE_
		ExportFile << "*" << Alphabet[i];
#endif		
	}
#ifdef _EXPORT_TO_FILE_
	ExportFile << "*" << endl;
#endif			
	cout << "*" << endl;
	//Draw the board
	for (int i = 0; i < this->Height; i++)
	{
		cout << Alphabet[i];
#ifdef _EXPORT_TO_FILE_
		ExportFile << Alphabet[i];
#endif
		for (int j = 0; j < this->Width; j++)
		{
#ifdef COLOR_MARKER
		switch(board[i][j])
		{
		case PLAYER1MARKER:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;

		case PLAYER2MARKER:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;

		default:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		}
#endif

			cout << " " << this->board[i][j];
#ifdef _EXPORT_TO_FILE_
			ExportFile << " " << this->board[i][j];
#endif			
		}
#ifdef COLOR_MARKER
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#endif
		cout << "*" << endl;
#ifdef _EXPORT_TO_FILE_	
		ExportFile << "*" << endl;
#endif	
	}
	//Draw last line
	for (int i = 0; i <= this->Width; i++)
	{
		cout << "**";
#ifdef _EXPORT_TO_FILE_	
		ExportFile << "**";
#endif	
	}
	cout << endl;
#ifdef _EXPORT_TO_FILE_	
		ExportFile << endl;
		ExportFile.close();
#endif	
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
	if ((this->board[Pos_R][Pos_C] == ' ') && (M != ' '))	//fill position
	{
		this->board[Pos_R][Pos_C] = M;
		uiCount++;
	}
	else if ((this->board[Pos_R][Pos_C] != ' ') && (M == ' '))	//clear position
	{
		this->board[Pos_R][Pos_C] = M;
		if (uiCount)
			uiCount--;
	}
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
#ifdef _EXPORT_TO_FILE_
	remove("gameTrace.txt");	//Delete the gameTrace history before starting
								//the new game
#endif
	//Input width, height, N of the board
	cout << "Please setup board's width, height and the number of point to win" << endl;

#ifdef _USE_EXTERNAL_FILE_
	fstream File;
	char strRead[30];
	
	File.open("conf.txt");
	if (File.is_open())	//Checking if the file exist
	{
		File.getline(strRead, 30);
		if (strcmp(strRead,"BTL-CK-Assignment02"))	//Checking header string
		{
			cout << "Header format incorrect." << endl << \
				 "Default setting is loaded: board 20x20, N = 5" << endl;
			this->Width = this->Height = 20; this->N = 5;
			File.getline(strRead, 30);	//ignore configuration in the file
			system("pause");
		}
		else
		{
			//Get board parameters
			File >> this->Width >> this->Height >> this->N;	//the last one is to skip the '\0' character
			if (File.fail())	//check
			{
				cout << "Config Error!" << endl;
				READFILEDATAFAIL;
			}
			//Check board parameters
			if ((this->Width < BOARD_MINN) || (this->Width > BOARD_MAXWIDTH) || \
				(this->Height < BOARD_MINN) || (this->Height > BOARD_MAXHEIGHT) || \
				(this->N < BOARD_MINN) || (this->N > BOARD_MAXN))
				{
					cout << "Board size out of range!!" << endl;
					READFILEDATAFAIL;
				}
			File.getline(strRead, 30);
			if (strRead[0] != '\0')		//Check if any character still exist behind the board's parameters
			{
				cout << "Format error!" << endl;
				READFILEDATAFAIL;
			}
		}
	}
	else
	{
		cout << "File not found!" << endl;
		return false;
	}
#else
config:	//Reset variable
	this->Width = 0; this->Height = 0; this->N = 0;

	//The program shall go nowhere if the user input not correct
	while ((this->Width > BOARD_MAXWIDTH) || (this->Width < BOARD_MINN))
	{
		cout << "Width = ";

		cin >> this->Width;
		if (cin.fail())
		{
			cout << "Please enter a number between " << BOARD_MINN << " and " \
				 << BOARD_MAXWIDTH << " !" << endl;
			cin.clear();
			cin.ignore(100, '\n');	 
		}
	}
	while ((this->Height > BOARD_MAXHEIGHT) || (this->Height < BOARD_MINN))
	{
		cout << "Height = ";

		cin >> this->Height;
		if (cin.fail())
		{
			cout << "Please enter a number between " << BOARD_MINN << " and " \
				 << BOARD_MAXHEIGHT << " !" << endl;
			cin.clear();
			cin.ignore(100, '\n');	 	 
		}
	}
	while ((this->N > BOARD_MAXN) || (this->N < BOARD_MINN))
	{
		cout << "N = ";

		cin >> this->N;
		if (cin.fail())
		{
			cout << "Please enter a number between " << BOARD_MINN << " and " \
				 << BOARD_MAXN << endl;
			cin.clear();
			cin.ignore(100, '\n');	 				 
		}
	}
#endif
	
	//Additional checking
	if ((this->Width < this->N) && (this->Height < this->N))
	{
		cout << "With your configuration. You will never be able to win the game." << \
			endl << "Try another configuration";
#ifdef _USE_EXTERNAL_FILE_
		cout << " and restart the game" << endl;
		READFILEDATAFAIL;
#else
		cout << endl;
		goto config;
#endif
	}
	uiCount = 0;
#ifdef _USE_EXTERNAL_FILE_
	//Get banned positions
	int BannedPos[100][2];
	while(!File.eof())	//Read until end of file
	{
		File >> strRead;
		if (strRead[0] == '\0')		//ignore the '\0' character
			continue;
		if ((strRead[0] != '(') || (strRead[4] != ')') || (strRead[2] != ',') || (strlen(strRead) > 5))	//check format
		{
			cout << "Banned position format incorrect. The correct format is (R,C)" << endl;
			READFILEDATAFAIL;
		}
		//Check if the postion is corrected
		BannedPos[uiCount][0] = ConvertChartoNum(strRead[1]);
		if ((BannedPos[uiCount][0] >= this->Height) || (BannedPos[uiCount][0] == -1))
		{
			cout << "Banned position iappropriated!" << endl;
			READFILEDATAFAIL;
		}
		BannedPos[uiCount][1] = ConvertChartoNum(strRead[3]);
		if ((BannedPos[uiCount][1] >= this->Width) || (BannedPos[uiCount][1] == -1))	
		{
			cout << "Banned position iappropriated!" << endl;
			READFILEDATAFAIL;
		}
		uiCount++; //increase the number of banned positions
	}
#endif
	this->board = new Marker*[this->Height];	//Make a new board
	for (int i = 0; i < this->Height; i++)
		this->board[i] = new Marker[this->Width];
	for (int i = 0, j = 0; (i < this->Height); (j < (this->Width - 1)) ? (j++) : (i++, j = 0))	//Initialize values for the board
		this->board[i][j] = ' ';		//By default, all values are empty. But for printing issue, the values is set to ' '

#ifdef _USE_EXTERNAL_FILE_	
	for (int i = 0; i < (int)uiCount; i++)
		this->board[BannedPos[i][0]][BannedPos[i][1]] = '#';	//import banned postions to board
	
	File.close();
#endif
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
	while (CheckProcess != COMPLETE)
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

/******************************************************************************
 * Convert a character to number.
 * Non case sensitive.
 * a or A = 0, b or B = 1 and so on
 *
 * Parameter:
 *  c: a character to be converted
 *
 * Return: number in type int
 *****************************************************************************/
inline int CaroBoard::ConvertChartoNum(char c)
{
	if ((c >= 'a') && (c <= 'z'))
		return (int)(c - 'a');
	if ((c >= 'A') && (c <= 'Z'))
		return (int)(c - 'A');
	else return -1;
}

/******************************************************************************
 * Setting up a blank board with known parameters
 *
 * Parameter:
 * 	Width : board width
 * 	Height: board height
 *
 * Return: none
 *****************************************************************************/
bool CaroBoard::ConfigBoard(int Width, int Height, int N)
{
	if ((N >= BOARD_MINN) && (N <= BOARD_MAXN))
	{
		if (((Width < N) && (Height < N)) || (Width > BOARD_MAXWIDTH) || (Height > BOARD_MAXHEIGHT))
			return false;
		else
		{
			this->Width = Width; this->Height = Height; this->N = N; uiCount = 0;
			this->board = new Marker*[Height];
			for (int i = 0; i < Height; i++)
				this->board[i] = new Marker[Width];
			
			for (int i = 0, j = 0; (i < this->Height); (j < (this->Width - 1)) ? (j++) : (i++, j = 0))
				this->board[i][j] = ' ';
			return true;
		}
	}
	else return false;
}

/* End of Board.cpp */
