# simple-vsat

A simple Verilog verification tool based on the Tseytin transformation and [MiniSat](minisat.se) solver.

What it does:
1. Checks reachability by parsing Verilog modules
2. Unrolls the transition relation and converts it to a dimacs formatted CNF
3. Calls MiniSat to solve satisfiability

## Requirments

- The Verilog file given to the program must be gate level.
- C++ 11 or newer
- CMake 3.10 or newer
- The desired end state of your outputs must be made as a comment at the end of the file. Your comment should be formated like this `//State:0100` with as many bits as there are outputs.
- Unrolls will be connected by register. Each register must bet set on a new line. ex/
```   
always @(posedge clock) begin
S1<=NS1;
S0<=NS0;
end
```

## Install

1. Clone this repo `git clone https://github.com/jforgue/simple-vsat.git`
2. Run CMake `cmake .`
3. Run Make `make`

## Run

simple-vsat takes two arguments, the path to your Verilog file and the number of times you want to unroll the Tseytin transition relation. The number of transition relation unrolls is 1 by default.

    ./simple-vsat <path-to-verilog> [number-of-relation-unrolls]
