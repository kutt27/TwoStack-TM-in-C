#include "tm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Creates a new TM, initializes its stacks, and loads the input string
TuringMachine* createTM(const char* input) {
    TuringMachine* tm = (TuringMachine*)malloc(sizeof(TuringMachine));
    if (tm == NULL) {
        perror("Failed to allocate Turing Machine");
        exit(1);
    }

    tm->stack1 = createStack(); // Left stack starts empty 
    tm->stack2 = createStack(); // Right stack holds the input
    tm->currentState = STATE_START; // Set initial state 

    // Load the input string onto stack2
    // We must push it in reverse order so the
    // first character of the string is at the top.
    int len = strlen(input);
    for (int i = len - 1; i >= 0; i--) {
        push(tm->stack2, input[i]);
    }
    
    // As per the paper's example, the tape often starts with a
    // blank symbol to mark the beginning[cite: 67, 102].
    // Let's use '_' as our explicit blank symbol.
    // The paper's example  has the head start on this
    // blank marker.
    // The example in [cite: 127] shows "Stack 2: _-1-1-..."
    // This implies the blank is at the top.
    
    // Let's refine this: The paper's example " |11 ..." 
    // with the head 's' under the blank, means stack2
    // should be [_, 1, 1].
    // Our current code loads "11", so stack2 is [1, 1].
    // Let's add the preceding blank.
    
    // Re-doing the loading logic to match the paper's example
    freeStack(tm->stack2); // Clear the one we just made
    tm->stack2 = createStack();
    
    // Push input string in reverse
    for (int i = len - 1; i >= 0; i--) {
        push(tm->stack2, input[i]);
    }
    // Push the initial blank symbol at the very beginning (top)
    // [cite: 67, 102, 127]
    push(tm->stack2, '_'); // Using '_' as our blank 

    return tm;
}

// Frees all memory associated with the TM
void freeTM(TuringMachine* tm) {
    freeStack(tm->stack1);
    freeStack(tm->stack2);
    free(tm);
}

// --- Helper Functions ---

// Helper function to print a stack (for printTape)
// Prints from the given node to the end
void printStack(Node* node) {
    Node* current = node; // Start from the node passed as an argument
    while (current != NULL) {
        printf("%c", current->data);
        current = current->next;
    }
}

// Helper function to print a stack in reverse (for printTape)
// Prints from bottom to top
void printStackReverse(Node* node) {
    if (node == NULL) {
        return;
    }
    printStackReverse(node->next);
    printf("%c", node->data);
}

// Prints the current state of the tape
void printTape(TuringMachine* tm) {
    // Print stack1 (left of head) in reverse order
    // (bottom-to-top) to get the correct tape representation
    printStackReverse(tm->stack1->top);
    
    // Print the "head" position
    printf(" [");
    // Print the symbol under the head (top of stack2)
    char headSymbol = peek(tm->stack2);
    if (headSymbol == '\0') {
        printf("_"); // Show blank if stack is empty
    } else {
        printf("%c", headSymbol);
    }
    printf("] ");

    // Print the rest of stack2 (right of head)
    if (tm->stack2->top != NULL) {
        printStack(tm->stack2->top->next);
    }
    printf("\n");
}

// Note: We've used _ as our explicit blank symbol to match the
// paper's description _ means empty cell and its examples. 
// I also adjusted the createTM function to add this initial blank
// symbol marker to the top of stack2, so the machine
// starts just like in the paper's trace: _ | 1 | 1 | ....

// Reads the symbol currently under the head (top of stack2)
char tm_read(TuringMachine* tm) {
    return peek(tm->stack2); // 
}

// Writes a symbol under the head (replaces top of stack2)
void tm_write(TuringMachine* tm, char symbol) {
    pop(tm->stack2); // Remove the old symbol 
    push(tm->stack2, symbol); // Push the new symbol 
}

// Moves the head one cell to the right
void tm_move_right(TuringMachine* tm) {
    // Pop from stack2, push to stack1 [cite: 132-133]
    char symbol = pop(tm->stack2);
    push(tm->stack1, symbol);
}

// Moves the head one cell to the left
void tm_move_left(TuringMachine* tm) {
    // Pop from stack1, push to stack2 
    char symbol = pop(tm->stack1);
    push(tm->stack2, symbol);
}

// Executes one transition step of the Turing Machine
// Returns 1 if a transition occurred, 0 if the machine is in the HALT state.
int tm_step(TuringMachine* tm) {
    if (tm->currentState == STATE_HALT) {
        return 0; // Machine is halted
    }

    // 1. Read Symbol
    char read_symbol = tm_read(tm);

    // 2. Lookup and Execute Transition (The Finite Automaton Logic)
    switch (tm->currentState) {
        
        case STATE_START:
            // start, _ -> move, _, R [cite: 92]
            if (read_symbol == '_') {
                // Write is _, so we don't need tm_write
                tm_move_right(tm);
                tm->currentState = STATE_MOVE;
            }
            break;

        case STATE_MOVE:
            if (read_symbol == '1') {
                // move, 1 -> 0, R (stay in move state) [cite: 93]
                tm_write(tm, '0');
                tm_move_right(tm);
                // State remains STATE_MOVE
            } 
            else if (read_symbol == '0') {
                // move, 0 -> back, 1, L [cite: 94]
                tm_write(tm, '1');
                tm_move_left(tm);
                tm->currentState = STATE_BACK;
            } 
            else if (read_symbol == '_') {
                // move, _ -> back, 1, L [cite: 95, 96]
                tm_write(tm, '1');
                tm_move_left(tm);
                tm->currentState = STATE_BACK;
            }
            break;

        case STATE_BACK:
            if (read_symbol == '0' || read_symbol == '1') {
                // back, 0/1 -> 0/1, L (Stay in back state and continue rewinding)
                // tm_write not needed since we aren't changing the symbol
                tm_move_left(tm);
            } 
            else if (read_symbol == '_') {
                // back, _ -> halt [cite: 98, 99]
                tm->currentState = STATE_HALT;
            }
            break;
            
        case STATE_HALT:
            // Should be caught by the initial check, but included for completeness.
            break;
    }

    return 1; // A step was executed
}