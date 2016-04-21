/*
 * RingStack.h - Header file of ring stack list
 *
 * Date: April 2016
 * Rev: 1.0
 * Author: Quang
 * Ho Chi Minh University of Technology
 */
 
#ifndef	__RINGSTACK_H__
#define __RINGSTACK_H__

/*******************************Definitions***********************************/
typedef struct Node {
	int Col, Row;
	Node *Next, *Pre;
};

/****************************Function prototypes******************************/
void Stack_Init();
void Stack_Push(int Row, int Col);
void Stack_Pop(int *Row, int *Col);
void Stack_Free();

#endif

/* End of RingStack.h*/

