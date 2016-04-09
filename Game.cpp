/*
 * Game.cpp - C++ Source file for handling the game states
 * 
 * Date: March 2015
 * Rev: 1.0
 * Author: Team 8 - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
*/

#include <iostream>
#include <stdint.h>
#include <string>
#include "Board.h"
#include "Game.h"
using namespace std;

/****************************Private Definitions******************************/
#define SKIPSPACECHAR	while(i < strInput.length()) if (strInput.at(i) == ' ') i++;else break
//#define _DEBUG_CHECKINGINPUT_

/*************************Private function prototypes*************************/
static bool Game_Playing_CheckingInput(string strInput, uint8_t *pRow, uint8_t *pCol);
static bool Game_Menu_CheckInput(uint8_t ui8Input);

/*********************************Variables***********************************/
extern CaroBoard board_c;
static bool bPlayer1Turn;

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
	cout << "Caro - Menu" << endl;
	cout << "1. Start new game" << endl;
	cout << "2. Information" << endl;
	cout << "3. Quit" << endl;

	//Let's the user choose the option
	do
	{
		cout << "Please pick your choice: ";
		cin >> ui8Input;
	} while (!Game_Menu_CheckInput(ui8Input));

	//Handle the input
	switch (ui8Input)
	{
		case '1':
			bPlayer1Turn = true;
			return GamePlay_Config;
			break;
			
		case '2':
			return Information;
			break;
			
		case '3':
		case 'q':
		case 'Q':
			return Quit;
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
	uint8_t ui8Row, ui8Col;
	//Draw the board
	board_c.DrawBoard();
	
	//Ask the player to input something
	do
	{
		strInput = "";
	cout << endl << "Player " << (-(int)bPlayer1Turn + 2) << " turn: ";
	while (strInput.length() == 0)
		getline(cin, strInput);
	//Loop until player input correctly
	} while (!Game_Playing_CheckingInput(strInput, &ui8Row, &ui8Col));
	
	//if player input 'q' or 'Q', the program will get out to menu immediately
	if (ui8Row == (uint8_t)'Q')
	{
		board_c.DestroyBoard();
		return Menu;
	}
	
	//Update the board after player has inputted
	board_c.UpdateBoard((int)ui8Row, (int)ui8Col, (bPlayer1Turn ? PLAYER1MARKER : PLAYER2MARKER));

	if (board_c.CheckFull())
	{
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

#ifdef _DEBUG_CHECKINGINPUT_
	cout << "First char != ' ', " << "i = " << i << endl << "Input length = " << strInput.length() << endl;
	system("pause");
#endif
	//if player input nothing, ask the player to input again
	if (i == strInput.length())
	{
		cout << "You input nothing. Please try again!" << endl;
		return false;
	}
	else if ((strInput.at(i) == 'q' || strInput.at(i) == 'Q') && (i == strInput.length() - 1))
	{
		*pRow = (uint8_t)'Q';
#ifdef _DEBUG_CHECKINGINPUT_
	cout << *pRow << endl;
	system("pause");
#endif
		return true;
	}
	else 
	{
#ifdef _DEBUG_CHECKINGINPUT_
		cout << "i = " << i << endl;
		cout << strInput.at(i) << endl;
		system("pause");
#endif
		int iBoardWidth, iBoardHeight;
		
		board_c.GetParameters(&iBoardWidth, &iBoardHeight);
#ifdef _DEBUG_CHECKWIDTHHEIGHT_
		cout << "width = " << iBoardWidth << "   " << "height = " << iBoardHeight << endl;
		system("pause");
#endif
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
			
		//return the Height position
#ifdef _DEBUG_CHECKINGINPUT_
		cout << "i = " << i << endl;
		cout << strInput.at(i) << endl;
		system("pause");
#endif
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

#ifdef _DEBUG_CHECKINGINPUT_
		cout << "Row = " << (int)*pRow << "  Col = " << (int)*pCol << endl;
		system("pause");
#endif
		
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
		 cout << "Input not correctly. Please type:" << endl << \
			"   1 to start a new game" << endl << \
			"   2 to see some information" << endl << \
			"   3 or q or Q to exit" << endl;
		 return false;
	 }
	 return true;
 }

/* End of Game.cpp */
