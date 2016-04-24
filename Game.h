/*
 * Game.h - Header file contain some definition and some function prototype
 * for handle the states of the game.
 *
 * Date: April 2016
 * Rev: 1.3
 * Author: Team 8
 * Group: TNMT - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
 * 
 * Revision History:
 * - 1.0: First release
 * - 1.1: add one more state - BeforeQuit, undefine PLAYER1X
 * - 1.2: define UNDOMOVE. This version allow user to undo moves
 * - 1.3: define MOVECURSOR. This version allow player to interract with program
 * via cursor
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
	
#define UNDOMOVE		//enable undo moves function
#define MOVECURSOR		//enable the cursor moving function

typedef enum 
{
	Menu = 0,
	GamePlay_Config,
	GamePlay_Playing,
	Information,
	BeforeQuit,
	Quit,
	Error = 0xFF
} State;

State Game_Menu(void);
State Game_Playing(void);
void Game_Information(void);

#endif

/* End of Game.h */
