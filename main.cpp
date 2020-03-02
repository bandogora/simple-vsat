/* What it does:
 * 1. Checks reachability by parsing Verilog modules
 * 2. Unrolls the transition relation and converts it to a dimacs formatted CNF
 * 3. Calls a SAT solver
 */

#include <to_dimacs.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <regex>
#include <parser.h>
#include <unistd.h>

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

// Check and return second argument number
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
  string line;
  int line_num = 1;
  ifstream f (argv[1]);

  // Check for first argument
  if (!argv[1]) {
    cout << "No file to read!" << endl;
    cout << "Usage: simple-vsat <path-to-verilog> [number-of-relation-unrolls]" << endl;
    exit(1);
  }

  // Verify first argument is a verilog file
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

  // Open verilog file
  if (!f.is_open()) {
    cout << "Error while opening file";
    exit(1);
  }

  // Create Parser instance
  Parse::parser file;

  // Parse the verilog file line by line
  cout << "Parsing..." << endl;
  while(getline(f, line)) {
    file.parse_line(line, line_num);
    line_num ++;
  }

  if (f.bad()) {
    cout << "Error while reading file";
    exit(1);
  }

  // Write our conversions to dimacs out file
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
