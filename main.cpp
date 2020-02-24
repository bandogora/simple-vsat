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
//#include <minisat/minisat/core/Main.h>

using namespace std;

ofstream outfile ("out.dimacs");

bool isNumber(const char number[]) {
  int i = 0;

  // Check for negative numbers
  if (number[0] == '-')
    i = 1;
  for (; number[i] != 0; i++) {
    //if (number[i] > '9' || number[i] < '0')
    if (!isdigit(number[i])) {
      return false;
    }
  }
  return true;
}

int main(int argc, char const *argv[]) {

  const regex verilog("^.*\.(vh|v)$");

  // Check first argument  is a verilog file
  if (!regex_match( argv[0], verilog)) {
      cerr << strerror(1) << "ERROR! Could not open file: " << argv[0] << endl;
  }

  // Check second argument is a number
  if (!isNumber(argv[1])) {
      cerr << strerror(1) << argv[1] << " is not a number" << endl;
  }

  string line;
  int line_num = 1;
  ifstream f (argv[0]);

  // Open verilog file
  if (!f.is_open()) {
      cerr << strerror(1) << "Error while opening file";
  }

  // Write fist line comment
  outfile << "c First line comment" << endl;

  // Let the user know whats happening
  cout << endl << "Parsing";
  while(getline(f, line)) {
    cout << '.';
    parse_line(line, line_num);
    line_num ++;
  }
  cout << endl;

  // Close dimacs file
  outfile.close();

  if (f.bad()) {
    cerr << strerror(1) << "Error while reading file";
  }
  return 0;
}
