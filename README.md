# Project README

## Overview
The project is a C program that implements a Pratt parser to evaluate mathematical expressions. It includes support for basic arithmetic operations and demonstrates its functionality through sample code.

## Features
- Implementing a Pratt parser.
- Support for basic arithmetic operations: addition, subtraction, multiplication, and division.
- Evaluation of mathematical expressions.
- Sample expressions provided in `code/Main`.

## Project Structure
### Prerequisites
- C/C++ Compiler (GCC)
- Make utility

## Build & Run
### Linux Build:
1. Clone or download the project files.
2. Navigate to the project directory.
3. Build the project using the following command:
   ```
   make -f Makefile.linux all
   ```
4. Execute the built program using:
   ```
   ./build/Main
   ```

### Windows Build:
1. Clone or download the project files.
2. Navigate to the project directory.
3. Build the project using the following command:
   ```
   make -f Makefile.windows all
   ```
4. Execute the built program using:
   ```
   .\build\Main.exe
   ```

### Web Assembly Build:
1. Clone or download the project files.
2. Navigate to the project directory.
3. Build the project using the following command:
   ```
   make -f Makefile.web all
   ```
4. Execute the built program using:
   ```
   wasmtime build/Main.wasm
   ```

### Wine Build (for Windows on Linux):
1. Clone or download the project files.
2. Navigate to the project directory.
3. Build the project using the following command:
   ```
   make -f Makefile.wine all
   ```
4. Execute the built program using:
   ```
   wine build/Main.exe
   ```

### Clean Build:
To clean the build artifacts and rebuild:
```
make -f Makefile.(os) clean
make -f Makefile.(os) all
```

This project demonstrates how to implement a Pratt parser for evaluating mathematical expressions in C.