#include <parser.h>
#include <iostream>
#include <string>
#include <regex>

using namespace Parse;
using namespace std;

const string delimiters = "(,";

void get_tokens(string str) {
  size_t pos = 0;
    string token;
    if ((pos = str.find(delimiters[0])) != string::npos) {
        token = str.substr(0, pos);
        cout << token << endl;
        str.erase(0, pos + 1);
    }
    while ((pos = str.find(delimiters[1])) != string::npos) {
      token = str.substr(0, pos);
      cout << token << endl;
      str.erase(0, pos + 1);
    }
  cout << str << endl;
}

// Time to reinvent that wheel, poorly!
void parser::parse_line(string& line, int& line_num) {
  // get just the symbol
  const regex symbol("^[^\s]*");

  switch (line[0]) {
  case 'm': // module
    // Erase beginning and end chars we don't want
    // i.e. "module test(in1,out1);" -> "test(in1,out1"
    line.erase(line.begin(), line.begin() + 7); // Erase symbol
    line.pop_back();
    line.pop_back();
    get_tokens(line);
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
        line.erase(line.begin(), line.begin() + 7); // Erase symbol
        line.pop_back();
    }
    else { // or
      // (~x + z)(~y + z)(x + y + ~z)
      line.erase(line.begin(), line.begin() + 7); // Erase symbol
      line.pop_back();
    }
    break;
  case 'r': // reg
    // We don't care about registers for dimacs
    break;
  case 'w': // wire
    line.erase(line.begin(), line.begin() + 5); // Erase symbol
    line.pop_back();
    break;
  case 'a':
    if (line[1] == 'n') { // and
      // (x + ~z)(y + ~z)(~x + ~y + z)
    }
    else { // always
      // We can ignore always tags
    }
    break;
  case 'n':
    if (line[1] == 'a') { // nand
      // (x + z)(y + z)(~x + ~y + ~z)
    }
    else { 
      if(line[2] == 't') { // not
        // (~a v ~x)(a v x)
      }
      else { // nor
        // (~x + ~z)(~y + ~z)(x + y + z)
      }
    }
    break;
  case 'x':
    if (line[1] == 'o') { // xor
      // (~x + y + z)(x + ~y + z)(~x + ~y + ~z)(x + y + ~z)
    }
    else { // xnor
      // (~x + ~y + z)(~x + y + ~z)(x + ~y + ~z)(x + y +z)
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

// Example Tseytin transformation
//
// and g1(a,s0,b)
// a = 1; s0 = 5; b = 2;
// (a v ~b)(s0 v ~b)(~a v ~s0 v b)

// c and g1
// 1 -2 0
// 5 -2 0
// -1 -5 2 0
