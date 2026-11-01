# Data-Structures-in-C
Low-level implementations of core data structures, hash tables, linked lists, and custom algorithms written in C.
A collection of custom data structures and low-level algorithms implemented from scratch in C, emphasizing explicit memory management, dynamic node allocation, and efficient data traversal.

| Project | Description | Key Tech / Concepts |
| :--- | :--- | :--- |
| **Arbitrary Precision Calculator (APC)** | A CLI application that performs arithmetic operations (`+`, `-`, `*`, `/`, `%`) on arbitrarily large numbers beyond standard integer data type limits[cite: 15]. Digits are stored across doubly linked list nodes for bidirectional computation, while results map to singly linked lists[cite: 15]. | C Programming, `struct`, Pointers, CLI (`argc`, `argv`), Doubly Linked Lists (DLL), Singly Linked Lists (SLL), Dynamic Memory (`malloc`, `free`), Pointer Manipulation, Carry/Borrow Logic, Arbitrary Precision Arithmetic[cite: 15]. |
| **Inverted Search Indexing System** | A command-line keyword search engine that indexes words across multiple text files using a hash table backed by linked lists[cite: 16]. Tracks word frequencies and file occurrences via nested main/sub-node architecture with persistent database save/update capabilities and formatted terminal displays[cite: 16]. | C Programming, `struct`, Pointers, Hash Tables with Chaining, Singly Linked Lists (SLL), Dynamic Memory (`malloc`, `free`), File I/O (`fopen`, `fscanf`, `fprintf`), String Handling (`string.h`), ANSI Escape Formatting, Search Algorithms[cite: 16]. |

## Tech Stack
* **Language:** C 
* **Environment:** Linux / WSL (Ubuntu)
* **Build Tool:** GNU Make / GCC