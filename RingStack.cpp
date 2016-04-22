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
 
#include "RingStack.h"

#define STACK_SIZE	50

static Node *pTop, *pBot;
static unsigned int Stack_uiCount;

void Stack_Init()
{
	pTop = pBot = NULL;
	Stack_uiCount = 0;
}

void Stack_Push( int Row, int Col)
{
	if (Stack_uiCount < STACK_MAXSIZE)
	{
		Node *newNode = new Node;
		newNode->Col = Col;
		newNode->Row = Row;
		newNode->pPre = pTop;
		pTop->pNext = newNode;
		newNode->pNext = NULL;
		pTop = newNode;
		if (!Stack_uiCount)
			pBot = newNode;
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
		pBot->pPre = NULL;
		pTop->pNext = NULL;
	}
}
void Stack_Pop(int *Row, int *Col)
{
	if (!Stack_uiCount)
		*Row = -1;
	else
	{
		*Row = pTop->Row;
		*Col = pTop->Col;
		pTop = pTop->pPre;
		delete pTop->pNext;
		pTop->pNext = NULL;
		Stack_uiCount--;
	}
}

void Stack_Free()
{
	for (int i = 1; i <= Stack_uiCount; i++)
	{
		pTop = pTop->pPre;
		delete pTop->pNext;
	}
	Stack_uiCount = 0;
}

/* End of RingStack.h*/