
# COP 290 Spreadsheet Program

This repository contains the source code and documentation for our memory-optimized, command-line spreadsheet program developed for COP 290. The project implements cell assignment, formula evaluation, and efficient recalculation using DFS-based cycle detection and topological sorting. Our final report (available as a PDF generated via `make report`) describes the design decisions, optimizations, and test cases in detail.

> **Note:**  
> The **Legacy** folder contains discarded code from earlier development phases, while the **Spreadsheet_Project** directory holds the current, actively maintained code.

## Repository Structure

The repository is organized into two main directories:
- **Legacy:** Contains discarded code from earlier development phases.
- **Spreadsheet_Project:** Contains the current, actively maintained code, including source files, headers, test files, and the LaTeX source for the final report.

## Building the Project

To build the project, follow these steps:

1. **Navigate to the project directory:**
   ```bash
   cd Spreadsheet_Project
   

2. **Compile the project:**
   ```bash
   make
   ```
   This command builds the executable and places it in the `target/release/` directory.

3. **Generate the project report:**
   ```bash
   make report
   ```
   This command produces a `main.pdf` file, which is our detailed final report.

## Running the Spreadsheet Program

After building the project, you can run the program using the following command:
```bash
./target/release/spreadsheet <ROWS> <COLS>
```
- `<ROWS>`: Number of rows for the spreadsheet (maximum 999).
- `<COLS>`: Number of columns for the spreadsheet (maximum 18,278).

**Example:**
```bash
./target/release/spreadsheet 100 26
```
The program launches in the terminal, allowing you to enter formulas, navigate the spreadsheet, and perform recalculations interactively.

## Features

- **Memory Optimized Data Storage:**  
  Uses short integers for indices and compact storage for dependency lists.

- **Dynamic Formula Evaluation:**  
  Supports integer constants, cell references, arithmetic expressions, and function calls (e.g., SUM, MIN).

- **Cycle Detection and Recalculation:**  
  Efficient DFS-based cycle detection prevents circular dependencies. Topological sorting ensures that only affected cells are recalculated after an update.

- **Robust Error Handling:**  
  Detects invalid inputs, out-of-range cells, and errors such as division by zero, propagating error flags to dependent cells.

- **Terminal-based User Interface:**  
  Includes commands for scrolling, toggling output, and jumping to specific cells.

## Design and Implementation

The design of the spreadsheet program is detailed in our final report (see [main.pdf](./main.pdf)). Key design highlights include:

- **Cell Representation:**  
  Each cell stores an integer value, a pointer to a formula structure, dependency lists, and flags for DFS and error detection.

- **Formula Parsing:**  
  Uses regular expressions to handle flexible input (supporting any number of spaces) and to distinguish among different formula types.

- **Dependency Management:**  
  Updates and recalculates cells in the correct order using DFS and topological sort, ensuring accurate and efficient computations.

- **Optimizations:**  
  Achieves compact storage for dependencies and efficient memory usage by using short integers and combined flag representations.

## Dependencies

- Standard C libraries and POSIX-compliant features.
- A C compiler (e.g., gcc) and the `make` utility for building the project.

## Acknowledgements

- **Instructor:** Abhilash Jindal
- **Group Members:**  
  - Aaditya Sharma (2023CS10420)  
  - Yash Rawat (2023CS50334)  
  - Ishan Rehal (2023CS10019)

For additional resources and a video demonstration of our project, please refer to the links provided in the final report.

