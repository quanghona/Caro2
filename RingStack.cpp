/*
 * RingStack.h - Header file of ring stack list. A ring stack is similar to
 * normal stack but when it overloaded, it will overwrite the bottom of the
 * stack and the bottom move up 1 level.
 * 
 * 
 * 	  Bottom = Top = nullptr     Bottom				Top	 Bottom
 *	  							|					 |	 |
 *	  							--------			--------	
 * 	  								    |			|		|
 *	  									| - Top		--------
 * 	  	Stack Empty				Normal case			Stack overloaded
 *
 * Note: When Stack is overloaded, the program not allocate new memory to the
 * stack but only replace its value
 *
 * Date: April 2016
 * Rev: 1.0
 * Author: Ly Hon Quang
 * e-mail: lyhonquang@gmail.com
 * Ho Chi Minh University of Technology
 */
 
#include "RingStack.h"
#include <stdint.h>

#define STACK_SIZE	50

static Node *pTop, *pBot;
static unsigned int Stack_uiCount;

/******************************************************************************
 * Initialize Stack
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void Stack_Init()
{
	//pTop = new Node;
	//pBot = new Node;
	Stack_uiCount = 0;
}

/******************************************************************************
 * Push stack
 *
 * Parameter:
 *	Row: row position
 *  Col: Column position
 *
 * Return: none
 *****************************************************************************/
void Stack_Push(uint8_t Row, uint8_t Col)
{
	if (Stack_uiCount < STACK_SIZE)
	{
		Node *newNode = new Node;
		newNode->Col = Col;
		newNode->Row = Row;
		newNode->pPre = pTop;
		newNode->pNext = nullptr;

		if (!Stack_uiCount)	//handle when Stack_uiCount 0->1
			pBot = newNode;	
		else pTop->pNext = newNode;
		
		pTop = newNode;

		Stack_uiCount++;
	}
	else
	{
		pBot->Col = Col;
		pBot->Row = Row;
		pBot->pPre = pTop;
		pTop->pNext = pBot;
		pTop = pBot;
		pBot = pBot->pNext;
		pBot->pPre = nullptr;
		pTop->pNext = nullptr;
	}
}

/******************************************************************************
 * Pop stack
 *
 * Parameter:
 *	*Row: pointer to row result
 *	*Col: pointer to Col result
 *
 * Return: none
 *****************************************************************************/
void Stack_Pop(uint8_t *Row, uint8_t *Col)
{
	if (!Stack_uiCount)
		*Row = (uint8_t)-1;
	else
	{
		*Row = pTop->Row;
		*Col = pTop->Col;
		if (pTop == pBot)
			pTop->pNext = pTop;	//handle the Stack_uiCount 1->0
		else
			pTop = pTop->pPre;
		delete pTop->pNext;
		if (pTop != pBot)
			pTop->pNext = nullptr;
		Stack_uiCount--;
	}
}

/******************************************************************************
 * Release Stack
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void Stack_Free()
{
	uint8_t temp = 0;
	while(Stack_uiCount) Stack_Pop(&temp, &temp);
	//delete pTop, pBot;
	//Stack_uiCount = 0;
}

/* End of RingStack.h*/