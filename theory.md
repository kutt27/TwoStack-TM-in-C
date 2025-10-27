## 1. What is a Turing Machine (TM)? 

The **Turing Machine** is a mathematical model of computation developed by Alan Turing in 1936. It is not a physical machine but an abstract representation of a computer program. Its purpose is to define and explore the limits of **computability**—what problems can, in principle, be solved by an algorithm.

A standard TM consists of three main components:

1.  **An Infinite Tape:** A tape divided into cells, where each cell holds a single symbol from a finite alphabet (e.g., '0', '1', and a blank symbol `_`). This tape serves as both the **input** (initially) and the **working memory** of the machine.
2.  **A Head:** A mechanism that reads the symbol on the cell it is currently positioned over, and can **write** a new symbol to that cell and **move** one cell left or right.
3.  **A Finite Control Unit (Finite Automaton):** This is the "brain" of the machine. It is always in one of a finite number of **states**. The control unit dictates the action of the machine based on its current state and the symbol it reads from the tape.

***

## 2. Standard TM Operations (The Transition Function)

The behavior of a TM is defined entirely by its **transition function** ($\delta$), which is a set of rules, or a **program**. Each rule takes the form:

$$\delta(\text{Current State}, \text{Read Symbol}) = (\text{New State}, \text{Write Symbol}, \text{Move Direction})$$

For every step of the computation, the machine performs the following three atomic operations:

| Operation | Description | Effect on the Tape/Control |
| :--- | :--- | :--- |
| **Read/Sense** | The head reads the symbol in the cell it is currently scanning. | Input to the control unit. |
| **Write** | The machine overwrites the current cell with a new symbol (which can be the same as the old one). | Changes the symbol on the tape. |
| **Move** | The head shifts one cell to the left ($L$) or one cell to the right ($R$). | Changes the symbol the head is scanning in the next step. |

The machine continues to execute these steps until it reaches a special **Halt State**, at which point the final content remaining on the tape is considered the output or result of the computation.

***

## 3. The Two-Stack Equivalent (The Paper's Proposal)

The paper's core contribution is a pedagogical one: It frames the Turing Machine in terms of a simpler concept students typically learn first—the **Pushdown Automaton (PDA)**.

### The Connection to Pushdown Automata

* A **Finite Automaton** recognizes only regular languages.
* A **Pushdown Automaton (PDA)** is a Finite Automaton equipped with **one stack**. PDAs can recognize context-free languages.
* The paper shows that a **Turing Machine** is equivalent to a Finite Automaton equipped with **two stacks**.

This relationship makes the TM seem like a natural, small step forward from the PDA, rather than an entirely new concept.

### The Role of the Two Stacks

The two stacks are used to model the **Infinite Tape** by logically splitting it at the position of the head.

| Component | Purpose in TM Model | Operation (Stack Analogy) |
| :--- | :--- | :--- |
| **Stack 1 (Left Stack)** | Represents the entire tape to the **left** of the head. | The symbol **immediately left** of the head is at the **top** of Stack 1. |
| **Stack 2 (Right Stack)** | Represents the symbol **under the head** and the entire tape to the **right**. | The symbol **under the head** is at the **top** of Stack 2. |

### Translation of TM Operations to Stack Operations

The standard TM operations are translated directly into simple stack manipulations, which is exactly what we implemented in the C code:

| TM Operation | Stack 1 (Left) | Stack 2 (Right) |
| :--- | :--- | :--- |
| **Read Symbol** | Peek | **Peek** |
| **Move Right** ($\mathbf{R}$) | $\text{Push}(\text{Pop from Stack 2})$ | $\text{Pop}$ |
| **Move Left** ($\mathbf{L}$) | $\text{Pop}$ | $\text{Push}(\text{Pop from Stack 1})$ |
| **Write Symbol** | **N/A** (Only affects symbol under head) | $\text{Pop}$, then $\text{Push}(\text{New Symbol})$ |

This two-stack approach proves the **computational equivalence** between the two models, demonstrating that the addition of a second stack is sufficient to grant the Pushdown Automaton the full power of the Turing Machine.