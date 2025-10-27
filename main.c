#include <stdio.h>
#include "tm.h"

// Helper function to convert a state enum to a string for printing
// ... (stateToString function remains the same) ...
const char* stateToString(State state) {
    switch (state) {
        case STATE_START: return "START";
        case STATE_MOVE:  return "MOVE";
        case STATE_BACK:  return "BACK";
        case STATE_HALT:  return "HALT";
        default:          return "UNKNOWN";
    }
}

int main() {
    const char* input = "11"; 
    printf("--- TM Simulation: Add 1 to Binary %s (3) ---\n", input);

    TuringMachine* tm = createTM(input);

    printf("Start State (S):\n");
    printf("  Tape: "); printTape(tm);
    printf("  State: %s\n", stateToString(tm->currentState));
    printf("----------------------------------\n");

    int step_count = 0;
    while (tm_step(tm)) {
        step_count++;
        printf("Step %2d (%c):\n", step_count, tm->currentState == STATE_MOVE ? 'm' : tm->currentState == STATE_BACK ? 'b' : 's');
        printf("  Tape: "); printTape(tm);
        printf("  State: %s\n", stateToString(tm->currentState));
        
        // This break prevents an infinite loop if a rule is missed or logic is wrong.
        if (step_count > 20) { 
            printf("ABORTING: Too many steps.\n"); 
            break; 
        }
    }
    
    printf("\n--- Computation Complete ---\n");
    printf("Final result (Head is at _):\n");
    // The final result should be read off the tape (Stack1 + Stack2 contents)
    printf("  Tape: "); printTape(tm);
    printf("  Final State: %s\n", stateToString(tm->currentState));
    printf("  Total Steps: %d\n", step_count);

    freeTM(tm);
    printf("\nCleanup complete.\n");

    return 0;
}