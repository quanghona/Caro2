/*
 * Game.h - Header file contain some definition and some function prototype
 * for handle the states of the game.
 *
 * Date: March 2015
 * Rev: 1.0
 * Author: Team 8 - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
*/

#ifndef __GAME_H__
#define __GAME_H__

/*Player marker type*/
//#define PLAYER1X
#ifdef PLAYER1X
#define PLAYER1MARKER			'X'
#define PLAYER2MARKER			'O'
#else
#define PLAYER1MARKER			'O'
#define PLAYER2MARKER			'X'	
#endif

typedef enum 
{
	Menu = 0,
	GamePlay_Config,
	GamePlay_Playing,
	Information,
	Quit,
	Error = 0xFF
} State;

//State GameState = Menu;
//CaroBoard board_c;

State Game_Menu(void);
State Game_Playing(void);
void Game_Information(void);


#endif

/* End of Game.h */
