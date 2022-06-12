#include <stdio.h>
#include <stdlib.h>

typedef struct Stack
{
    int *value;
    int length;
    int max_length;
} Stack;

Stack * createStack(int size)
{

    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->max_length = size;
    stack->length = 0;
    stack->value = (int*)malloc(sizeof(int) * size);
    return stack;
}

int pushStack(Stack *stack, int item)
{
    if (stack->length == stack->max_length)
    {
        int new_stack[stack->max_length * 2];

        for (int i = 0; i < stack->max_length; i++)
        {
            new_stack[i] = stack->value[i];
        }

        stack->max_length *= 2;
        stack->value = (int*)malloc(sizeof(int) * stack->max_length);

        stack->value = new_stack;

        stack->value[stack->length] = item;
        stack->length++;
    }
    else
    {
        stack->value[stack->length] = item;
        stack->length++;
    }

    return stack->length;
}

int popStack(Stack *stack)
{
    if(stack->length != 0) {
        stack->length--;
        stack->value[stack->length] = 0x0;
    }
}

int peekStack(Stack *stack) {
    return stack->value[stack->length - 1];
}