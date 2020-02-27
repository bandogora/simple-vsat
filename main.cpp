/* What it does:
 * 1. Checks reachability by parsing Verilog modules
 * 2. Unrolls the transition relation and converts it to a dimacs formatted CNF
 * 3. Calls a SAT solver
 */

/* Requirements:
 * 1. It takes two inputs from command line:
 *      i. the name of the Verilog file to read
 *      ii. the number of times to unroll the transition relation in the symbolic search.
 *
 * 2. The SAT solver will check whether the formula is satisfiable, and the result will
 *    reveal whether the target state is reachable in some number of transitions from the initial state.
 */

/*
 * Note: The target states listed in the table above are reproduced as a comment
 *       at the bottom of each Verilog file so that your program can read it from there.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <parser.h>
//#include <minisat.h> some how include minisat

using namespace Parse;
using namespace std;

int isNumber(const char str[]) {
  string numString;
  int i = 0;
  while(isdigit(str[i])) {
    numString += str[i];
    i++;
  }
  int num = atoi(numString.c_str());
  return num;
}

int main(int argc, char const *argv[]) {
  // Create regex for file ending
  const regex v_file("^.*\.(vh|v)$");

  // Check first argument  is a verilog file
  smatch m;
  string str = argv[1];
  if (!regex_match(str, m, v_file)) {
    cout << "ERROR! Could not open file: " << argv[1] << endl;
    exit(1);
  }
  else {
    cout << "is verilog file" << endl;
  }

  // Check second argument is a number
  if (!isNumber(argv[2])) {
    cout << argv[2] << " is not a positive integer" << endl;
    exit(1);
  }
  // Initiate vars
  string line;
  int line_num = 1;
  ifstream f (argv[1]);

  // Open verilog file
  if (!f.is_open()) {
    cout << "Error while opening file";
    exit(1);
  }

  // Create Parser instance
  parser file;

  // Let the user know whats happening
  cout << "Parsing..." << endl;
  while(getline(f, line)) {
    file.parse_line(line, line_num);
    line_num ++;
  }

  if (f.bad()) {
    cout << "Error while reading file";
    exit(1);
  }
  return 0;
}
