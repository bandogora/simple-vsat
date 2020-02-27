# simple-vsat

A simple Verilog verification tool based on the Tseytin transformation and [MiniSat](minisat.se) solver.

## Requirments

- The verilog file given to the program must be gate level.
- Currently restrained to two input gates.
- C++ 11 or newer
- CMake 3.10 or newer

## Install

1. Clone this repo `git clone https://github.com/jforgue/simple-vsat.git`
2. Run CMake `cmake .`
3. Run Make `make`
