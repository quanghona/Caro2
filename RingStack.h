/*
 * RingStack.h - Header file of ring stack list. A ring stack is similar to normal stack
 * but when it overloaded, it will overwrite the bottom of the stack and the bottom move up 1 level
 * 
 * 
 * 	  Bottom = Top = NULL     Bottom				Top	 Bottom
 *	  							|					 |	 |
 *	  							--------			--------	
 * 	  								    |			|		|
 *	  									| - Top		--------
 * 	  	Stack Empty				Normal case			Stack overloaded
 *
 * Note: When Stack is overloaded, the program not allocate new memory to the stack but only replace
 * its value
 *
 * Date: April 2016
 * Rev: 1.0
 * Author: Ly Hon Quang
 * e-mail: lyhonquang@gmail.com
 * Ho Chi Minh University of Technology
 */

#include <stdint.h>
 
#ifndef	__RINGSTACK_H__
#define __RINGSTACK_H__

/*******************************Definitions***********************************/
typedef struct NODE{
	int Col, Row;
	NODE *pNext, *pPre;
} Node;

/****************************Function prototypes******************************/
void Stack_Init();
void Stack_Push(uint8_t Row, uint8_t Col);
void Stack_Pop(uint8_t *Row, uint8_t *Col);
void Stack_Free();

#endif

/* End of RingStack.h*/

