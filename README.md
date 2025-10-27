### Compilation & Execution

```bash
# Compile all .c files and link them into an executable named 'tm_test'
gcc main.c tm.c stack.c -o tm_test'

# or
clang main.c tm.c stack.c -o tm_test # if you have clang installed

# Run the executable
./tm_test
```

##### Paper Reference

- Authors: Vladik Kreinovich and Olga Kosheleva 
T- itle: A Turing Machine Is Just a Finite Automaton with Two Stacks: A Comment on Teaching Theory of Computation 
- Source/Publication: Published in the Proceedings of the 8th International Scientific-Practical Conference "Mathematical Education in Schools and Universities: Innovations in the Information Space" MATHEDU 2018 
- Location and Date of Conference: Kazan, Russia, October 17-21, 2018 
- Pages: pp. 152-156 
- Technical Report Number (UTEP): UTEP-CS-18-54 
- Year: 2018

## Theory

[Core Theory](theory.md)

According to the paper, our "machine" is a Finite Automaton equipped with two stacks. The Stacks (The "Tape"): These two stacks will represent the Turing Machine's infinite tape.

- stack1 (Left Stack): Contains everything on the tape before the head. The symbol immediately to the left of the head is at the top of this stack.
- stack2 (Right Stack): Contains the symbol the head is currently pointing to, plus everything after it. The symbol under the head is at the top of this stack.

The Finite Automaton (The "Control Unit"): This is the "brain" of the machine. At any given moment, it's in one of a finite set of states. For our C implementation, this will simply be a variable that holds the currentState of the machine (e.g., START, MOVE, BACK, HALT as used in the paper's example ).

#### Implementation:
Let's create the C structures for these components. We'll need a Stack [stack-header](stack.h) and the main TuringMachine [turing-machine-header](tm.h) structure that holds everything together.

For the stack, a linked list is a good choice because it can grow and shrink dynamically, just like the "tape" our stacks are supposed to represent.

## Implementing the Stack and TM Initialization

1. Stack Implementation: We're using a standard singly linked list as a stack.

    - `push` adds a new node to the head (top) of the list.
    - `pop` removes the node from the head (top) and returns its data.
    - `peek` just looks at the head's data without removing it.

2. TM Initialization: When we create the Turing Machine, we need to load the input string onto its "tape" (our two stacks). Based on the paper's model :

    - The machine starts with the head pointing to the first cell of the input.
    - Therefore, the left stack (`stack1`) starts empty.
    - The right stack (`stack2`) gets the entire input string, with the first character of the string at the top of the stack .
    - The paper also mentions a "blank" symbol (like `_`). We must handle the case where the input tape is empty or the head moves to an unused part of the tape. For now, our `pop` and `peek` functions will return `\0` (null char) if the stack is empty, which we can treat as our "blank" symbol.
    - The finite automaton (control unit) will be set to `STATE_START`.

Check: [stack.c](stack.c) & [tm.c](tm.c)

Now we can create the main.c file to tie everything together and test our initialization. This file will:
- Include our Turing Machine header `tm.h`.
- Define the input string "11" (representing the binary number 3, as in the paper's example ).
- Call `createTM()` to set up the machine with this input.
- Print the initial state of the tape using our `printTape()` helper.
- Clean up the allocated memory by calling `freeTM()`.

[main.c](main.c)

#### Cross-validation:

Correct output:

```bash                         
Initializing Turing Machine with input: 11
Initial State:
  Finite Automaton State: START
  Tape:  [_] 11

Cleanup complete.
```

This confirms our setup is correct:

- `stack1` (left of head) is empty.
- `stack2` (at/right of head) contains _, 1, 1.
- The head is pointing at the _ (the top of `stack2`).
- The automaton is in `STATE_START`.

## Implementing the Basic TM Operations

Now we translate the fundamental Turing Machine actions (Move Left, Move Right, Write, Read) into operations on our two stacks. The paper describes this clearly:

1. Read Symbol: The symbol currently under the head is simply the symbol at the top of `stack2`. We just need to `peek` at `stack2`.
2. Move Right: To move the head one cell to the right, we:
    - `pop` the top symbol from `stack2` (the one currently under the head).
    - `push` that same symbol onto `stack1` (it is now just to the left of the head) .
3. Move Left: To move the head one cell to the left, we:
    -`pop` the top symbol from `stack1` (the one immediately to the left).
    - `push` that same symbol onto `stack2` (it is now under the head).
4. Write Symbol: To replace the symbol under the head, we:
    - `pop` the top symbol from `stack2` (discarding it).
    - `push` the new symbol onto `stack2`.

Handling Infinite "Blank" Tape: The tape is infinite, but our stacks are finite. We need to handle moving onto "empty" parts of the tape.

- The paper mentions adding a blank symbol when a part of the tape is used for the first time.
- We'll formalize this: If we `pop` or `peek` from an empty stack, it's a "blank". We'll use `_` as our explicit blank symbol.
- Let's update our `stack.c` to return `_` instead of `\0` when empty. This makes it consistent with our tape representation.

#### Cross-validation:

Correct output:

```bash
Initializing Turing Machine with input: 11
Initial State:
  State: START
  Tape:   [_] 11
  Read:  _
----------------------------------
Action: Move Right
  State: START
  Tape:  _ [1] 1
  Read:  1
----------------------------------
Action: Write '0'
  State: START
  Tape:  _ [0] 1
  Read:  0
----------------------------------
Action: Move Left
  State: START
  Tape:   [_] 01
  Read:  _
----------------------------------

Cleanup complete.
```

## Implementing the Finite Automaton (Control Unit)

The control unit is implemented as a single function, often called `tm_step()`, which represents one single step of computation.

The steps for a transition are:

1. Read: Get the symbol under the head (top of stack2).
2. Lookup: Find the rule that matches the current State and the Read Symbol.
3. Execute: Perform the actions defined by the rule:
    - Write: Replace the current symbol (or leave it unchanged).
    - Move: Move the head Left (L) or Right (R).
    - State Change: Transition to the next state.

We will use the rules for the "add 1 to a binary number" example provided in the paper .

|Current State|Read Symbol|Write Symbol|Move|Next State|FIELD6|
|-------------|-----------|------------|----|----------|------|
|start        |_          |_           |R   |          |move  |
|move         |1          |0           |R   |          |move  |
|move         |0          |1           |L   |          |back  |
|move         |_          |1           |L   |          |back  |
|back         |0          |0           |L   |          |back  |
|back         |1          |1           |L   |back      |      |
|back         |_          |_           |N/A |          |halt  |

Note: The paper's rules are slightly abbreviated; we'll interpret back, 0 -> L as "read 0, write 0, move L, stay in back state" and the final rule as "read $\text{_}$, stay $\text{_}$, move $\text{N/A}$, go to halt."

#### Cross-validation:

```bash
--- TM Simulation: Add 1 to Binary 11 (3) ---
Start State (S):
  Tape:  [_] 11
  State: START
----------------------------------
Step  1 (m):
  Tape: _ [1] 1
  State: MOVE
Step  2 (m):
  Tape: _0 [1] 
  State: MOVE
Step  3 (m):
  Tape: _00 [_] 
  State: MOVE
Step  4 (b):
  Tape: _0 [0] 1
  State: BACK
Step  5 (b):
  Tape: _ [0] 01
  State: BACK
Step  6 (b):
  Tape:  [_] 001
  State: BACK
Step  7 (s):
  Tape:  [_] 001
  State: HALT

--- Computation Complete ---
Final result (Head is at _):
  Tape:  [_] 001
  Final State: HALT
  Total Steps: 7

Cleanup complete.
```