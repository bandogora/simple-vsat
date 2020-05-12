#include <to_dimacs.h>
#include <iostream>
#include <parser.h>
#include <regex>
#include <string>
#include <unordered_map>

using namespace Parse;
using namespace std;

extern vector<string> out;
extern unordered_map<string, int> wires;
extern vector<array<string, 2>> reg_wire_state;
extern int unroll_num;

// Get just the symbol
const regex symbol("^[^\\s]*");

// Find the register at the beginning of the line
const regex reg("(.+?)(?=<)");

// Find the wire following the register
const regex wire("^.+?[=]");

void parser::parse_line(string& line, int& line_num) {
  // Init convert class
  Convert::convert new_line;

  switch (line[0]) {
    case 'm': // module
      out.push_back("c " + line);
      break;
    case 'i': // input
      out.push_back("c " + line);

      // Send line to record inputs
      new_line.get_dimacs(line, 0);

      // Send line again so inputs can also be wires
      new_line.get_dimacs(line, 8);
      break;
    case 'o':
      if (line[1] == 'u') { // output
        out.push_back("c " + line);
        new_line.get_dimacs(line, 0);
        new_line.get_dimacs(line, 9);
      }
      else { // or
        out.push_back("c unrolling: " + line);
        new_line.get_dimacs(line, 1);
      }
      break;
    case 'r': // reg
      new_line.get_dimacs(line, 10);
      break;
    case 'w': // wire
      new_line.get_dimacs(line, 0);
      break;
    case 'a':
      if (line[1] == 'n') { // and
        out.push_back("c unrolling: " + line);
        new_line.get_dimacs(line, 2);
      }
      // We can ignore always tags
      break;
    case 'n':
      if (line[1] == 'a') { // nand
        out.push_back("c unrolling: " + line);
        new_line.get_dimacs(line, 3);
      }
      else { 
        if(line[2] == 't') { // not
          out.push_back("c unrolling: " + line);
          new_line.get_dimacs(line, 4);
        }
        else { // nor
          out.push_back("c unrolling: " + line);
          new_line.get_dimacs(line, 5);
        }
      }
      break;
    case 'x':
      if (line[1] == 'o') { // xor
        out.push_back("c unrolling: " + line);
        new_line.get_dimacs(line, 6);
      }
      else { // xnor
        out.push_back("c unrolling: " + line);
        new_line.get_dimacs(line, 7);
      }
      break;
    case 'e': // end
      // We can ignore end tags
      break;
    case '/': // comment
      if (line[2] == 'S') { // State string
            new_line.set_state(line);
      }
      // We can ignore comments
      break;
    default:
      smatch m;
      // Check for register set
      if (regex_search(line, m, reg)) {
        auto it = wires.find(m.str(0) += "0");
        if (it != wires.end()) {
          line.pop_back();
          reg_wire_state.push_back({m.str(0), regex_replace(line, wire, "")});
          break;
        }
      }
      // Symbol not recognized so ignore it
      else if (regex_match(line, m, symbol)) {
        cout << "Unrecognized symbol '" << m.str(0) << "' on line " << line_num << ", ignoring" << endl;
      }
      else {
        cout << "Unrecognized symbol on line " << line_num << ", ignoring" << endl;
      }
      break;
  }
}
