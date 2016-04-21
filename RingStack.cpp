#include <RingStack.h>

#define STACK_MAXSIZE	50

static Node *Top, *Bot;
static unsigned int Stack_CurrentSize;

void Stack_Init()
{
	Top = Bot = NULL;
	Stack_CurrentSize = 0;
}

void Stack_Push( int Row, int Col)
{
	if (Stack_CurrentSize < STACK_MAXSIZE)
	{
		Node *newNode = new Node;
		newNode->Col = Col;
		newNode->Row = Row;
		newNode->Pre = Top;
		Top->Next = newNode;
		newNode->Next = NULL;
		Top = newNode;
		if (!Stack_CurrentSize)
			Bot = newNode;
		Stack_CurrentSize++;
	}
	else
	{
		Bot->Col = Col;
		Bot->Row = Row;
		Bot->Pre = Top;
		Top->Next = Bot;
		Top = Bot;
		Bot = Bot->Next;
		Bot->Pre = NULL;
		Top->Next = NULL;
	}
}
void Stack_Pop(int *Row, int *Col)
{
	if (!Stack_CurrentSize)
		*Row = -1;
	else
	{
		*Row = Top->Row;
		*Col = Top->Col;
		Top = Top->Pre;
		delete Top->Next;
		Top->Next = NULL;
		Stack_CurrentSize--;
	}
}

void Stack_Free()
{
	for (int i = 1; i <= Stack_CurrentSize; i++)
	{
		Top = Top->Pre;
		delete Top->Next;
	}
	Stack_CurrentSize = 0;
}

/* End of RingStack.h*/