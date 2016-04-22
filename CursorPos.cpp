/*
 * CursorPos.h - Handle the task related to the cusor position
 *
 * Date: April 2016
 * Rev: 1.0
 * Author: Ly Hon Quang
 * e-mail: lyhonquang@gmail.com
 * Ho Chi Minh University of Technology
 */

#include "CursorPos.h"
#include <Windows.h>

/******************************************************************************
 * Set the Cusor Position in console window
 *
 * Parameter:
 *	column: the column position. Start = 0
 *	line  : the row position. Start = 0
 *
 * Return: none
 *****************************************************************************/
void SetCurPos( uint8_t column, uint8_t line )
  {
  COORD coord;
  coord.X = column;
  coord.Y = line;
  SetConsoleCursorPosition(
    GetStdHandle( STD_OUTPUT_HANDLE ),
    coord
    );
  }

/******************************************************************************
 * Get the current position of the cusor
 *
 * Parameter:
 *	x: x coordinate of the cusor
 *	y: x coordinate of the cusor
 *
 * Return:
 *	0 if the position is detectable
 *	-1 if the process fail
 *****************************************************************************/
int GetCurPos(uint8_t &x, uint8_t &y)
  {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(
         GetStdHandle( STD_OUTPUT_HANDLE ),
         &csbi
         ))
    return -1;
  x = (uint8_t)csbi.dwCursorPosition.X;
  y = (uint8_t)csbi.dwCursorPosition.Y;
 return 0;
  }
