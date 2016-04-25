/*
 * Caro - A simple game writing in C++ console programming.
 * This game operate state by state. Every state has specific functions
 *  1. Menu: At this state, there are 3 option: Start new game, information
 * and Quit.
 *   1.1 Start new game: players start a new game.
 * 	 1.2 information   : print the introduction and some informations of the 
 * game.
 *   1.3 Quit: Quit the game.
 *  2. Information: print the introduction, the rules and some other features.
 * After this, the player will return to main menu.
 *  3. Game Config: Setup the board before playing the game.
 *  4. Game Play  : 2 players playing turn by turn.
 *  5. Quit       : Quit the game.
 * 
 * Feature: 
 *  - Undo move
 *  - Control using cursor
 *  - Graphics, Color
 *  - Save history
 *  - Config using external file
 * 
 * Date: April 2016
 * Rev: 1.2
 * Author: Team 8
 * Group: TNMT - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
 *
 * Revision History:
 *  - 1.0: First release
 *  - 1.1: fixed main()
 *  - 1.2: little fix
 *  - 1.3: add one more state: BeforeQuit
*/

#include "Game.h"
#include "Board.h"
#include <iostream>
#include <stdint.h>
#include <Windows.h>
#include <conio.h>
using namespace std;
State GameState = Menu;
CaroBoard board_c;

void main()
{
	//The program running according to the current state of the game
	while (GameState != Quit)
	{
		system("cls");
		switch (GameState)
		{
			case Menu:
				GameState = Game_Menu();
				break;
				
			case GamePlay_Config:
				GameState = board_c.ConfigBoard() ? GamePlay_Playing : (system("pause"), BeforeQuit);
				break;
				
			case GamePlay_Playing:
				GameState = Game_Playing();
				break;
				
			case Information:
				Game_Information();
				
				cout << "Press any key to back to Main menu." << endl;
				_getch();
				GameState = Menu;
				break;
				
			case BeforeQuit:
				cout << endl << "Thanks for playing this game!!" << endl;
				GameState = Quit;
				break;
				
			case Error:		//This case should never happen
				cout << "Unexpected Error occurred!" << endl;
				break;
		}
	}
	Sleep(1000);
}

/* End of Caro.cpp */
