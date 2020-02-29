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

#include <to_dimacs.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <regex>
#include <parser.h>
#include <unistd.h>
//#include <minisat.h> some how include minisat

using namespace std;

// Create regex for file ending
const regex v_file("^.*\.(vh|v)$");

int unroll_num;
extern int pvals [2];
extern vector<string> out;

void write_dimacs() {
  // Open new dimacs file for writing
  ofstream outfile ("out.dimacs");

  // Write p line
  outfile << "p cnf " << pvals[0] << " " << pvals[1] << endl;

  // Write remaining lines in order
  for (vector<string>::iterator it = out.begin(), end = out.end(); it != end; ++it) {
    outfile << *it << endl;
  }

  // Close dimacs file
  outfile.close();

  cout << "Dimacs file written to './out.dimacs'" << endl;
}

int isNumber(const char str[]) {
  string numString;
  int i = 0;
  while(isdigit(str[i])) {
    numString += str[i];
    i++;
  }
  unroll_num = atoi(numString.c_str());
  return unroll_num;
}

int main(int argc, char const *argv[]) {
  // Check first argument  is a verilog file

  if (!argv[1]) {
    cout << "No file to read!" << endl;
    cout << "Usage: simple-vsat <path-to-verilog> [number-of-relation-unrolls]" << endl;
    exit(1);
  }

  smatch m;
  string str = argv[1];
  if (!regex_match(str, m, v_file)) {
    cout << "ERROR! Could not open file: " << argv[1] << endl;
    exit(1);
  }

  // Check second argument is a number
  if(!argv[2]) {
    unroll_num = 1;
  }
  else if (!isNumber(argv[2])) {
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
  Parse::parser file;

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

  write_dimacs();

  // Call MiniSAT
  int pid, status;
  if (pid = fork()) {
    waitpid(pid, &status, 0);
  }
  else {
    const char exec[] = "./minisat/minisat";
    execl(exec, exec, "./out.dimacs", NULL);
  }

  return 0;
}
