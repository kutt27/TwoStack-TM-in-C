#ifndef TM_H
#define TM_H

#include "stack.h"

// Define the states for our Finite Automaton
// We'll use the states from the paper's example 
typedef enum {
    STATE_START,
    STATE_MOVE,
    STATE_BACK,
    STATE_HALT
} State;

// The main Turing Machine structure
typedef struct TuringMachine {
    Stack* stack1;      // Represents the tape to the LEFT of the head 
    Stack* stack2;      // Represents the tape AT and to the RIGHT of the head 
    State currentState; // The current state of the finite automaton 
} TuringMachine;

// --- TM Operations ---

// Creates a new TM, initializes its stacks, and loads the input string
TuringMachine* createTM(const char* input);

// Frees all memory associated with the TM
void freeTM(TuringMachine* tm);

// A helper function to print the current state of the tape
// (Useful for debugging)
void printTape(TuringMachine* tm);

// --- New TM Operations ---

// Reads the symbol currently under the head (top of stack2)
char tm_read(TuringMachine* tm);

// Writes a symbol under the head (replaces top of stack2)
void tm_write(TuringMachine* tm, char symbol);

// Moves the head one cell to the right
void tm_move_right(TuringMachine* tm);

// Moves the head one cell to the left
void tm_move_left(TuringMachine* tm);

// Executes one transition step of the Turing Machine
// Returns 1 if a transition occurred, 0 if the machine is in the HALT state.
int tm_step(TuringMachine* tm);

#endif // TM_H
