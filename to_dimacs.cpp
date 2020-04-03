// Example Tseytin transformation
//
// input x,y;
// output z;
// and g1(z,x,y)
// a = 1; s0 = 5; b = 2;
// (x v ~z)(y v ~z)(~x v ~y v z)

// c and g1
// 1 -2 0
// 5 -2 0
// -1 -5 2 0

#include <to_dimacs.h>
#include <iostream>
#include <map>
#include <parser.h>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

using namespace Convert;
using namespace std;

unordered_map<string, int> wires;
int wire_num = 1;
extern int unroll_num;
vector<string> registers;
vector<array<string, 2>> reg_wire_state;

// Write lines to vector so the file can begin with p line
vector<string> out;

// Count number of vars and clauses for p line
int pvals[2];

// Find the begining 
const regex find_wires("([^,()\\s]+)(?=\\s*\\)|\\s*,|\\s*;)");

// Get digits from state line
const regex digit("\\d");

// Store wires as "cnf" numbers
vector<string> cnf;

void get_wires(int gate, string wire) {
  // Do this for as many times as unrolls were specified
  for (int k = 0; k < unroll_num; k++) {
    if (!gate || (gate == 10)) {
      // Store each "wire" in a map as a name and identifying number
      wires.insert({wire + to_string(k), wire_num});
      wire_num++;
    }
    else {
      // Push number that corresponds to wire name to vector for later use
      cnf.push_back(to_string(wires.find(wire + to_string(k))->second));
    }
  }
}

void convert::get_dimacs(string& str, int gate) {
  // Regex match store 
  smatch m;

  // Maintain string minus the symbol for additional runs
  string new_str = str;

  // Find wires and put into cnf vector
  while(regex_search(str, m, find_wires)) {
    get_wires(gate, m.str(0));
    str = m.suffix(); 
  }

  // Once more to get last wire
  // regex_search(str, m, find_wires);
  // get_wires(gate, m.str(0));


  // Do this for as many times as unrolls were specified
  for (int j = 0; j < unroll_num; j++) {
    // For each gate type create the Tseytin translation and use
    // the previously stored numbers to write as dimacs
    switch (gate) {
      case 1: // or
        // (~x + z)(~y + z)(x + y + ~z)
        out.push_back("-"+ cnf[2 + j]+ " "+ cnf[0 + j]+ " 0");
        out.push_back("-"+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        out.push_back(cnf[2 + j]+ " "+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");

        // Count number of clauses
        pvals[1] += 3;
        break;
      case 2: // and
        // (x + ~z)(y + ~z)(~x + ~y + z)
        out.push_back(cnf[2 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back(cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back("-"+ cnf[2 + j]+ " -"+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        pvals[1] += 3;
        break;
      case 3: // nand
        // (x + z)(y + z)(~x + ~y + ~z)
        out.push_back(cnf[2 + j]+ " "+ cnf[0 + j]+ " 0");
        out.push_back(cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        out.push_back("-"+ cnf[2 + j]+ " -"+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
        pvals[1] += 3;
        break;
      case 4: // not
        // (~x v ~z)(x v z)
        out.push_back("-"+ cnf[2 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back(cnf[2 + j]+ " "+ cnf[0 + j]+ " 0");
        pvals[1] += 2;
        break;
      case 5: //nor
        // (~x + ~z)(~y + ~z)(x + y + z)
        out.push_back("-"+ cnf[2 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back("-"+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back(cnf[2 + j]+ " "+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        pvals[1] += 3;
        break;
      case 6: //xor
        // (~x + y + z)(x + ~y + z)(~x + ~y + ~z)(x + y + ~z)
        out.push_back("-"+ cnf[2 + j]+ " "+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        out.push_back(cnf[2 + j]+ " -"+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        out.push_back("-"+ cnf[2 + j]+ " -"+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back(cnf[2 + j]+ " "+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
        pvals[1] += 4;
        break;
      case 7: // xnor
        // (~x + ~y + z)(~x + y + ~z)(x + ~y + ~z)(x + y + z)
        out.push_back("-"+ cnf[2 + j]+ " -"+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        out.push_back("-"+ cnf[2 + j]+ " "+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back(cnf[2 + j]+ " -"+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
        out.push_back(cnf[2 + j]+ " "+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
        pvals[1] += 4;
        break;
      default: break;
    }
  }
  switch (gate) {
      case 8: // input
      // Initialize input states
      // Do this for as many times as unrolls were specified
      for (int m = 0; m < cnf.size(); m++) {
        out.push_back("-" + cnf[m] + " 0");
        pvals[1] += 1;
      }
      break;
    case 9: // out
      // Initialize output states
      // for (int m = 0; m < cnf.size(); m++) {
      //   out.push_back(cnf[m]+ " 0");
      //   pvals[1] += 1;
      // }
      break;
    case 10: // reg
      registers = cnf;
      break;
    default: break;
  }
  str = new_str;
  pvals[0] = wires.size();
}

void convert::set_state(string& reg) {
  smatch m;

  // Saves state bits to vector
  vector<string> states;
  while(regex_search(reg, m, digit)) {
    states.push_back(m.str(0));
    reg = m.suffix(); 
  }
  for (int j = 0; j < reg_wire_state.size(); j++) {
    if (unroll_num > 1) {
      for (int k = 0; k < unroll_num - 1; k++) {
        out.push_back("c connect reg " + reg_wire_state[j][0] + to_string(k) + " to reg " + reg_wire_state[j][0] + to_string(k + 1));
        string first_wire = to_string(wires.find(reg_wire_state[j][0] + to_string(k))->second);
        string second_wire = to_string(wires.find(reg_wire_state[j][1] + to_string(k + 1))->second);
        out.push_back(first_wire + " " + second_wire + " 0");
      }
    }
    out.push_back("c set final reg " + reg_wire_state[j][1] + to_string(unroll_num - 1) + " to state given");
    if (states[j] == "1") {
      out.push_back(to_string(wires.find(reg_wire_state[j][1] + to_string(unroll_num - 1))->second) + " 0");
    }
    else if (states[j] == "0") {
     out.push_back("-" + to_string(wires.find(reg_wire_state[j][1] + to_string(unroll_num - 1))->second) + " 0");
    }
  }
}
