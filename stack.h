#include <stdio.h>
#include <stdlib.h>

typedef struct Stack
{
    int *value;
    int length;
    int max_length;
} Stack;

Stack *createStack(int size)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->max_length = size;
    stack->length = 0;
    stack->value = (int *)malloc(sizeof(int) * size);

    return stack;
}

int pushStack(Stack *stack, int item)
{
    if (stack->length == stack->max_length)
    {
        int new_stack[stack->max_length * 2];

        stack->max_length *= 2;

        stack->value = (int *)realloc(stack->value, sizeof(int) * stack->max_length);

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
    if (stack->length != 0)
    {
        stack->length--;

        if (stack->max_length != 2)
        {
            if (stack->length == stack->max_length / 2)
            {
                stack->max_length /= 2;
                stack->value = (int *)realloc(stack->value, sizeof(int) * stack->max_length);
            }
        }
    }
}

int peekStack(Stack *stack)
{
    return stack->value[stack->length - 1];
}

void deleteStack(Stack *stack)
{
    free(stack->value);
    free(stack);
}