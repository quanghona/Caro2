/*
 * Game.cpp - C++ Source file for handling the game states
 * 
 * Date: April 2016
 * Rev: 1.4
 * Author: Team 8
 * Group: TNMT - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
 * 
 * Revision History:
 *  - 1.0: First release
 *  - 1.1: update Game_Information(), fix and update Game_Playing_CheckingInput()
 * 			fix Game_Menu()
 * 	- 1.2: update Game_Information(), update Game_Playing(), update Game_Playing_CheckingInput()
 * this revision allow user able to undo moves
 *  - 1.3: Update Game_Playing(), this version allow player to move the cursor on the screen,
 * and interract with program via that cursor
 *  - 1.4: fix bug, make program more virtualized
*/

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Board.h"
#include "Game.h"

#ifdef UNDOMOVE
#include "RingStack.h"
#endif
#ifdef MOVECURSOR
#include "CursorPos.h"
#define KEY_UP		72
#define KEY_LEFT	75
#define KEY_RIGHT	77
#define KEY_DOWN	80
#define KEY_ENTER	13
#endif
using namespace std;

/****************************Private Definitions******************************/
#define SKIPSPACECHAR	while(i < strInput.length()) if (strInput.at(i) == ' ') i++;else break
#define SAMPLEBOARD_HEIGHT	5
#define SAMPLEBOARD_WIDTH	8

/*************************Private function prototypes*************************/
static bool Game_Playing_CheckingInput(string strInput, uint8_t *pRow, uint8_t *pCol);
static bool Game_Menu_CheckInput(uint8_t ui8Input);
static void Game_Menu_SetScreenSize( unsigned width, unsigned height );

/*********************************Variables***********************************/
extern CaroBoard board_c;
static bool bPlayer1Turn;
const char CaroImg[][29] = {"  xxx    oo   xxxx     ooo  ",
							" x   x  o  o  x   x   o   o ",
							"x      o    o x    x o     o",
							"x      o    o x   x  o     o",
							"x      oooooo xxxx   o     o",
							"x      o    o x   x  o     o",
							" x   x o    o x    x  o   o ",
							"  xxx  o    o x    x   ooo  "};

/****************************Function definitions*****************************/

/******************************************************************************
 * Handle the Menu State
 * In this state, player have various options to choose:
 * 	1. Start new game: when choosing this option, a player will had to config
 * some parameters of the board before starting the game.
 *  2. Information: see the introduction, rules ...
 *  3. Quit: exit
 *
 * Parameter: none
 *
 * Return: the next state depend on player selection
 *  GamePlay_Config: config a board before starting a new game
 *  Information    : print the introduction and instruction of the game
 *  Quit		   : Exit the game
 *****************************************************************************/
State Game_Menu(void)
{
	uint8_t ui8Input = 0;
	
	//Print the menu for user to choose;
	//system("cls");
	Game_Menu_SetScreenSize(70, 30);

	for (int i =  0; i < 8; i++)
	{
		SetCurPos(22, i+3);
		for (int j = 0; j < 29; j++)
			cout << CaroImg[i][j];
	}
	//make the cursor disapear
	PCONSOLE_CURSOR_INFO cci = new CONSOLE_CURSOR_INFO;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), cci);
	cci->bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), cci);

	cout << endl;
	SetCurPos(25, 15);
	cout << "1. Start new game";
	SetCurPos(25, 16);
	cout << "2. Information";
	SetCurPos(25, 17);
	cout << "3. Quit";

	//Let's the user choose the option
	do
	{
		ui8Input = _getch();
	} while (!Game_Menu_CheckInput(ui8Input));

	//Handle the input
	switch (ui8Input)
	{
		case '1':
			bPlayer1Turn = true;
#ifdef UNDOMOVE
			Stack_Init();	//initialize stack
#endif
			//Set the cursor visible again before switching to play game mode
			cci->bVisible = true;
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), cci);
			delete cci;
			return GamePlay_Config;
			
		case '2':
			delete cci;
			return Information;
			
		case '3':
		case 'q':
		case 'Q':
			return BeforeQuit;
			break;
		
		default:		//Input error
			return Error;
	}
}

/******************************************************************************
 * Control the state of playing the game.
 * First it will print the board. Then it ask the player to input a position to
 * mark. A program will handle the input and check if the player win or not. If
 * so, the game end. Else, switch player and the game continue.
 *
 * Parameter: none
 *
 * Return: The next state will be executed
 *****************************************************************************/
State Game_Playing(void)
{
	string strInput;
	uint8_t ui8Row = 0, ui8Col = 0;
	//Draw the board
	system("cls");
	board_c.DrawBoard();
	
	//Ask the player to input something
#ifdef MOVECURSOR
	cout << endl << "Player " << (-(int)bPlayer1Turn + 2) << " turn: ";
	SetCurPos(2, 1);
	int Width, Height;
	board_c.GetParameters(&Width, &Height);
	do {
		char cInput = _getch();	//input
		switch(cInput)		//handle arrow keys and some special cases
		{
		case KEY_UP:
		case 'w':
		case 'W':
			if (ui8Row)	ui8Row--;
			break;

		case KEY_DOWN:
		case 's':
		case 'S':
			if (ui8Row < Height - 1)	ui8Row++;
			break;

		case KEY_LEFT:
		case 'a':
		case 'A':
			if (ui8Col)	ui8Col--;
			break;

		case KEY_RIGHT:
		case 'd':
		case 'D':
			if (ui8Col < Width - 1)		ui8Col++;
			break;

		case '!':
		case '~':
			ui8Row = 'Q';
			break;
#ifdef UNDOMOVE
		case '-':
			ui8Row = '-';
			break;
#endif
		case KEY_ENTER:
			break;

		default:
			continue;
		}
		//Escape the loop if special case detected
		if (cInput == KEY_ENTER)
		{
			if (!board_c.CheckMarkPos((int)ui8Row,(int)ui8Col))
				continue;
			else break;
		}
		else if (ui8Row == 'Q' || ui8Row == '-')
			break;
		else SetCurPos(2+2*ui8Col, ui8Row + 1);	//execute the arrow key command
	} while(1);
#else

	do
	{
		strInput = "";
	cout << endl << "Player " << (-(int)bPlayer1Turn + 2) << " turn: ";
	while (strInput.length() == 0)
		getline(cin, strInput);
	//Loop until player input correctly
	} while (!Game_Playing_CheckingInput(strInput, &ui8Row, &ui8Col));
#endif

	//if player input 'q' or 'Q', the program will get out to menu immediately
	switch (ui8Row)
	{
		case ((uint8_t)'Q'):
			board_c.DestroyBoard();
#ifdef UNDOMOVE
			Stack_Free();
#endif
			return Menu;
			break;
#ifdef	UNDOMOVE		
		case ((uint8_t)'-'):		//if player press '-', it will undo 1 move
			Stack_Pop(&ui8Row, &ui8Col);
			if (ui8Row != (uint8_t)-1)
				board_c.UpdateBoard((int)ui8Row, (int)ui8Col, ' ');
			else{
#ifdef MOVECURSOR
				SetCurPos(0, Height + 4);
#endif

				cout << "No move to undo!" << endl;		//Stack empty
				Sleep(500);
				return GamePlay_Playing;
			}
			break;
#endif
		default:
		//Update the board after player has inputted
		board_c.UpdateBoard((int)ui8Row, (int)ui8Col, (bPlayer1Turn ? PLAYER1MARKER : PLAYER2MARKER));
#ifdef UNDOMOVE
		Stack_Push(ui8Row, ui8Col);
#endif
	}

	if (board_c.CheckFull())
	{
		system("cls");
		board_c.DrawBoard();
		cout << "Game end. No one win this game!" << endl;
		goto newgame;
	}
	
	//Checking the winning condition
	if (board_c.CheckBoard(ui8Row, ui8Col, (bPlayer1Turn ? PLAYER1MARKER : PLAYER2MARKER)))
	{
		system("cls");
		board_c.DrawBoard();
		cout << endl << "Player " << (-(int)bPlayer1Turn + 2) << " win the game!! " << endl;
		//system("pause");
		//Ask players if they want to start another game
newgame:		cout << endl << "Do you want to start a new game?(y/n) ";
		char cAns = 0;
		while ((cAns != 'y') && (cAns != 'Y') && (cAns != 'n') && (cAns != 'N'))
			cin >> cAns;
		board_c.DestroyBoard();
#ifdef UNDOMOVE
		Stack_Free();	//release stack
#endif
		return (((cAns == 'y') || (cAns == 'Y')) ? GamePlay_Config : Menu);
	}
	
	//if not satisfied the condition, the game continue and switch turn
	bPlayer1Turn = !bPlayer1Turn;
	return GamePlay_Playing;
}

/******************************************************************************
 * Print the information and playing instruction of the game.
 * After this, the game should be return to Menu.
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void Game_Information(void)
{	
	cout << endl << "Caro is a popular game for every one" << endl << endl;
	
	cout << "The rule is very simple. 2 player play turn by turn. Each turn, a player" << endl << \
	"will choose a position to mark in. the winner is the one who has his/her" << endl << \
	"markers aligned continously vertical/horizontal/diagonal. The number of " << endl << \
	"continous markers to win the game can be set by player." << endl << endl;
	
	cout << "Before playing the game, player should be setup some neccessary" << endl << \
	"information like the number of the board's width and height." << endl << endl;
	cout << "Note: The maximun number of height and width are 26 and 25 respectively." << \
	"And the number of markers to win the game have to be between 3 and 5." << endl;
	cout << "Player 1 Marker is O, and player 2 is X." << endl << endl;
	
	//print sample board
	CaroBoard SampleBoard;
	
	SampleBoard.ConfigBoard(SAMPLEBOARD_WIDTH, SAMPLEBOARD_HEIGHT, 5);
	int RandomPos[2][9];	//get 9 random position to fill marker
	for (int i = 0; i < 9; i++)
	{
		RandomPos[0][i] = rand() % SAMPLEBOARD_HEIGHT;
		RandomPos[1][i] = rand() % SAMPLEBOARD_WIDTH;
		for (int j = 0; j < i; j++)
			if ((RandomPos[0][i] == RandomPos[0][j]) && (RandomPos[1][i] == RandomPos[1][j]))
			{
				i--;		//repeat until get a unique position
				break;
			}
	}	
	for (int i = 0; i < 9; i++)
		SampleBoard.UpdateBoard(RandomPos[0][i], RandomPos[1][i], (i < 3 ? PLAYER1MARKER : (i < 6 ? PLAYER2MARKER : '#')));
	
	SampleBoard.DrawBoard();
	
	cout << endl << "Player 1 turn:" << Alphabet[rand() % SAMPLEBOARD_HEIGHT] \
		 << ", " << Alphabet[rand() % SAMPLEBOARD_WIDTH] << endl <<  endl;
	SampleBoard.DestroyBoard();
}

/******************************************************************************
 * Checking the input condition.
 *
 * Parameter:
 * 	strInput: the input string to be checked
 * 	*pRow    : get the row position from the string or the quit command
 * 	*pCol    : get the column position from the string
 *
 * Return:
 * 	true: input correctly and result saved to *pRow and *pCol
 * 	false: input error
 *****************************************************************************/
static bool Game_Playing_CheckingInput(string strInput, uint8_t *pRow, uint8_t *pCol)
{
	unsigned int i = 0;
	
	//find the first char which is not a space
	SKIPSPACECHAR;

	//if player input nothing, ask the player to input again
	if (i == strInput.length())
	{
		cout << "You input nothing. Please try again!" << endl;
		return false;
	}
	else if ((strInput.at(i) == '~' || strInput.at(i) == '!'))
	{
		*pRow = (uint8_t)'Q';
		return true;
	}
#ifdef	UNDOMOVE
	else if (strInput.at(i) == '-')
	{
		*pRow = (uint8_t)'-';
		return true;
	}
#endif
	else 
	{
		int iBoardWidth, iBoardHeight;
		
		board_c.GetParameters(&iBoardWidth, &iBoardHeight);
		//return the Row position
		if (((strInput.at(i) >= 'A') && (strInput.at(i) < ('A' + (char)iBoardWidth))))
			*pRow = (uint8_t)(strInput.at(i++) - 'A');
		else if ((strInput.at(i) >= 'a') && (strInput.at(i) < ('a' + (char)iBoardWidth)))
			*pRow = (uint8_t)(strInput.at(i++) - 'a');
		else goto InErr;

Nxtchr:	//Find the next character
		SKIPSPACECHAR;
		//if player input not enough argument, ask the player to input again
		if (i == strInput.length()) goto InErr;
		//Ignoring ','
		if (strInput.at(i) == ',')
		{
			i++;
			goto Nxtchr;
		}
		
		//Check if player input more than 2 argument
		unsigned int *temp = new unsigned int;
		*temp = i++;	//save the current position and detect the remaining string if
						//is any character left
		SKIPSPACECHAR;
		if (i < strInput.length())
		{
			cout << "Too much argument. Input again!" << endl;
			delete temp;
			return false;
		}
		i = *temp;
		delete temp;
			
		//return the Height position
		//Return the Column position
		if (((strInput.at(i) >= 'A') && (strInput.at(i) < ('A' + (char)iBoardHeight))))
			*pCol = (uint8_t)(strInput.at(i++) - 'A');
		else if ((strInput.at(i) >= 'a') && (strInput.at(i) < ('a' + (char)iBoardHeight)))
			*pCol = (uint8_t)(strInput.at(i++) - 'a');
		else goto InErr;

		if (!board_c.CheckMarkPos((int)*pRow, (int)*pCol))
		{
			cout << "This position has already filled. Pick another position!" << endl;
			return false;
		}		
		return true;
	}
	
InErr:
	cout << "Input Error. Please try again!" << endl;
	return false;
}

 /******************************************************************************
 * Check the user input when at menu.
 *
 * Parameter:
 * 	ui8Input: the input value to be checked
 *
 * Return:
 * 	true: input correctly
 * 	false: input error
 *****************************************************************************/
 static bool Game_Menu_CheckInput(uint8_t ui8Input)
 {
	 if (((ui8Input < '1') || (ui8Input > '3')) && (ui8Input != 'q') && (ui8Input != 'Q'))
	 {
		 cout << endl << "Input not correctly. Please type:" << endl << \
			"   1 to start a new game" << endl << \
			"   2 to see some information" << endl << \
			"   3 or q or Q to exit" << endl;
		 return false;
	 }
	 return true;
 }

static void Game_Menu_SetScreenSize( unsigned width, unsigned height )
{
	SMALL_RECT r;
	COORD      c;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConOut = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo( hConOut, &csbi );

	r.Left = r.Top = 0;
	r.Right  = width - 1;
	r.Bottom = height - 1;
	SetConsoleWindowInfo( hConOut, true, &r );

	c.X = width;
	c.Y = height;
	//SetConsoleScreenBufferSize( hConOut, c );
}

/* End of Game.cpp */
