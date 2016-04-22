/*
 * CursorPos.h - header file to handle the task related to the cusor position
 *
 * Date: April 2016
 * Rev: 1.0
 * Author: Ly Hon Quang
 * e-mail: lyhonquang@gmail.com
 * Ho Chi Minh University of Technology
 */

#include <stdint.h>

#ifndef __CURSORPOS_H__
#define __CURSORPOS_H__

void SetCurPos(uint8_t column, uint8_t line );
int GetCurPos(uint8_t &x, uint8_t &y);

#endif