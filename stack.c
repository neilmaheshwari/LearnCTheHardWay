#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct Frame {
    struct Frame *next;
    int data;
};

struct Stack {
    struct Frame *head;
    int size;
};

struct Stack *new();

int pop(struct Stack *stack);

void push(struct Stack *stack);

void free_stack(struct Stack *stack);

void _die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}


void main(int argc, char *argv[])
{
    
}
