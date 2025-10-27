#ifndef STACK_H
#define STACK_H

// A single node in our stack (linked list)
typedef struct Node {
    char data;          // The symbol on the tape (e.g., '0', '1', or '_' for blank)
    struct Node* next;
} Node;

// The Stack structure itself (just a pointer to the top)
typedef struct Stack {
    Node* top;
} Stack;

// --- Stack Operations ---

// Creates and returns a new, empty stack
Stack* createStack();

// Pushes a new symbol onto the top of the stack
void push(Stack* stack, char data);

// Pops and returns the top symbol from the stack
// Returns '\0' (null terminator) if the stack is empty
char pop(Stack* stack);

// Returns the top symbol without removing it
// Returns '\0' (null terminator) if the stack is empty
char peek(Stack* stack);

// Checks if the stack is empty
int isEmpty(Stack* stack);

// Frees all memory associated with the stack
void freeStack(Stack* stack);

#endif // STACK_H
