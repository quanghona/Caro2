/*
 * Board.h - Header file of the board library
 * Handle everything related to the board in the game Caro
 * 
 * Date: March 2016
 * Rev: 1.1
 * Author: Team 8 - Fundamental of C++ Programming
 * Ho Chi Minh University of Technology
 * 
 * Revision History:
 *  - 1.0: First release
 *  - 1.1: fixed function configBoard()
*/

#ifndef __BOARD_H__
#define __BOARD_H__

#define _USE_EXTERNAL_FILE_		//define this when config board using external txt file

#define BOARD_MAXWIDTH			26
#define BOARD_MAXHEIGHT			25
#define BOARD_MAXN				5
#define BOARD_MINN				3

typedef char Marker;

class CaroBoard			//A class handle everything related to the caro-board
{
	public:
		bool ConfigBoard(void);
		void DrawBoard(void);
		void UpdateBoard(int Pos_R, int Pos_C, Marker M);
		bool CheckBoard(int Pos_R, int Pos_C, Marker CurPlyM);
		bool CheckMarkPos(int Pos_R, int Pos_C);
		bool CheckFull(void);
		void DestroyBoard(void);
		void GetParameters(int *Width, int *Height);
	private:
		int Width, Height, N;
		Marker **board;
};

#endif

/* End of Board.h */
