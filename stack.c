#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

// Creates and returns a new, empty stack
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        perror("Failed to allocate stack");
        exit(1);
    }
    stack->top = NULL;
    return stack;
}

// Pushes a new symbol onto the top of the stack
void push(Stack* stack, char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Failed to allocate node");
        exit(1);
    }
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

// Pops and returns the top symbol from the stack
// Returns '_' (our "blank" symbol) if the stack is empty
char pop(Stack* stack) {
    if (isEmpty(stack)) {
        return '_'; // Representing blank   
    }
    Node* temp = stack->top;
    char data = temp->data;
    stack->top = temp->next;
    free(temp);
    return data;
}

// Returns the top symbol without removing it
// Returns '_' (our "blank" symbol) if the stack is empty
char peek(Stack* stack) {
    if (isEmpty(stack)) {
        return '_'; // Representing blank
    }
    return stack->top->data;
}

// Checks if the stack is empty
int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

// Frees all memory associated with the stack
void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}