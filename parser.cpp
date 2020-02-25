#include <parser.h>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <vector>

using namespace Parse;
using namespace std;

string seperate(stringstream ss) {

  vector<int> vect;

  for (int i; ss >> i;) {
    vect.push_back(i);
    while (ss.peek() != '(') {
      ss.ignore();
    }
    if (ss.peek() == ',') {
        ss.ignore();
    }
  }
}

// Time to reinvent that wheel, poorly!
void parser::parse_line(string& line, int& line_num) {
  // get just the symbol
  const regex symbol("^[^\s]*");

  stringstream ss(line);

  switch (line[0]) {
  case 'm': // module
    // Erase beginning and end chars we don't want
    // i.e. "module test(in1,out1);" -> "test(in1,out1)"
    line.erase(line.begin(), line.begin() + 7); // Erase symbol
    line.pop_back();
    cout << line << endl; // debug
    break;
  case 'i': // input
    // "input in1,in2;" -> "in1,in2"
    line.erase(line.begin(), line.begin() + 6); // Erase symbol
    line.pop_back();
    cout << line << endl; // debug
    break;
  case 'o':
    if (line[1] == 'u') { // output
        
    }
    else { // or
      
    }
    break;
  case 'r': // reg
    break;
  case 'w': // wire
    break;
  case 'a':
    if (line[1] == 'n') { // and
    }
    else { // always
    }
    break;
  case 'n':
    if (line[1] == 'a') { // nand
    }
    else { 
      if(line[2] == 't') { // not
      }
      else { // nor
      }
    }
    break;
  case 'x':
    if (line[1] == 'o') { // xor
    }
    else { // xnor
    }
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


