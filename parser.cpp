#include <to_dimacs.h>
#include <iostream>
#include <parser.h>
#include <regex>
#include <string>

using namespace Parse;
using namespace std;

// Time to reinvent that wheel, poorly!
void parser::parse_line(string& line, int& line_num) {
  // Init convert class
  Convert::convert new_line;

  // get just the symbol
  const regex symbol("^[^\s]*");

  switch (line[0]) {
  case 'm': // module
    // Erase beginning and end chars we don't want
    // i.e. "module test(in1,out1);" -> "test(in1,out1"
    cout << ("c " + line) << endl;
    break;
  case 'i': // input
    // "input in1,in2;" -> "in1,in2"
    cout << ("c " + line) << endl;
    line.erase(line.begin(), line.begin() + 6);
    line.pop_back();
    new_line.get_dimacs(line, 0);
    // Send again to initialize inputs
    new_line.get_dimacs(line, 8);
    break;
  case 'o':
    if (line[1] == 'u') { // output
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 7);
      line.pop_back();
      new_line.get_dimacs(line, 0);
      // Send again to initialize outputs
      new_line.get_dimacs(line, 9);
    }
    else { // or
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 3);
      line.pop_back();
      line.pop_back();
      new_line.get_dimacs(line, 1);
    }
    break;
  case 'r': // reg
    cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      new_line.get_dimacs(line, 0);
    break;
  case 'w': // wire
    cout << ("c " + line) << endl;
    line.erase(line.begin(), line.begin() + 5);
    line.pop_back();
    new_line.get_dimacs(line, 0);
    break;
  case 'a':
    if (line[1] == 'n') { // and
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      line.pop_back();
      new_line.get_dimacs(line, 2);
    }
      // We can ignore always tags
    break;
  case 'n':
    if (line[1] == 'a') { // nand
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 5);
      line.pop_back();
      line.pop_back();
      new_line.get_dimacs(line, 3);
    }
    else { 
      if(line[2] == 't') { // not
        cout << ("c " + line) << endl;
        line.erase(line.begin(), line.begin() + 4);
        line.pop_back();
        line.pop_back();
        new_line.get_dimacs(line, 4);
      }
      else { // nor
        cout << ("c " + line) << endl;
        line.erase(line.begin(), line.begin() + 4);
        line.pop_back();
        line.pop_back();
        new_line.get_dimacs(line, 5);
      }
    }
    break;
  case 'x':
    if (line[1] == 'o') { // xor
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      line.pop_back();
      new_line.get_dimacs(line, 6);
    }
    else { // xnor
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 5);
      line.pop_back();
      line.pop_back();
      new_line.get_dimacs(line, 7);
    }
    break;
  case 'e': // end
    // We can ignore end tags
    break;
  default:
    // Symbol not recognized so ignore it
    smatch m;
    if (regex_match(line, m, symbol)) {
      cout << "Unrecognized symbol '" << m[0] << "' on line " << line_num << ", ignoring" << endl;
    }
    else {
      cout << "Unrecognized symbol on line " << line_num << ", ignoring" << endl;
    }
    break;
  }
}
