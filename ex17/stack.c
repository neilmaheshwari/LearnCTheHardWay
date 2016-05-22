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

void push(struct Stack *stack, int item);

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

struct Stack *new()
{
    // Allocate memory for the stack
    struct Stack *stack = malloc(sizeof(struct Stack));

    if(!stack) {
	_die("Could not allocate memory for stack");
    };

    // Set the pointer to the next frame to null since
    // the the created stack is empty. Initialize the
    // size field to 0 since the stack is empty.
    struct Frame *stack_head = NULL;
    stack->head = stack_head;
    stack->size = 0;

    return stack;
}

int pop(struct Stack *stack)
{
    // Check if the stack is null or empty
    if(!stack) {
	_die("Could not pop null stack");
    };
    if(stack->size <= 0) {
	_die("Could not pop empty satck");
    };

    // Get data and next frame of the stack
    struct Frame *head = stack->head;
    if(!head) {
	_die("Head should not be null for non empty stack");
    };
    int item = head->data;
    struct Frame *next = head->next;

    // Update the stack
    stack->size = stack->size - 1;
    stack->head = next;

    // Free the memory allocated for the previous head
    free(head);
    
    // Return the popped element
    return item;
}

void push(struct Stack *stack, int item)
{
    // Check if stack is null
    if(!stack) {
	_die("Could not push item onto null stack");
    };

    // Construct new head frame
    struct Frame *new_head = malloc(sizeof(struct Stack));
    if(!new_head) {
	_die("Could not allocate memory for head of stack");
    };
    new_head->data = item;
    new_head->next = stack->head;

    // Update the stack
    stack->size = stack->size + 1;
    stack->head = new_head;
}

void free_stack(struct Stack *stack)
{
    if(stack) {
	
	int i = 0;

	// Pop each element in the stack to
	// free the memoory allocated per frame
	int stack_size = stack->size;
	for(i = 0; i < stack_size; i++) {
	    int tmp = pop(stack);
	}

	// Free the stack struct itself
	free(stack);
    };
}

int main(int argc, char *argv[])
{
    struct Stack *my_stack = new();

    push(my_stack, 0);
    push(my_stack, 1);
    push(my_stack, 2);

    // Only pop two elements so free has to
    // de-allocate the stack
    int fst = pop(my_stack);
    int snd = pop(my_stack);

    printf("First element %d\n", fst);
    printf("Second element %d\n", snd);
    
    free_stack(my_stack);
    
    return 0;
}
