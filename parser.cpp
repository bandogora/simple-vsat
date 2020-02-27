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

#include <fstream>
#include <iostream>
#include <parser.h>
#include <regex>
#include <string>
#include <unordered_map>

using namespace Parse;
using namespace std;

// Open new dimacs file for writing
ofstream outfile ("out.dimacs");

const string delimiters = "(,";
unordered_map<string, int> wires;
int wire_num = 1;

void get_tokens(string& str, int cnf, bool wire = false) {
  size_t pos;
  string token;
  string new_line;
  
  if ((pos = str.find(delimiters[0])) != string::npos) {
    token = str.substr(0, pos);
    str.erase(0, pos + 1);
  }

  while ((pos = str.find(delimiters[1])) != string::npos) {
      token = str.substr(0, pos);
      if (wire) {
        wires.insert({token, wire_num});
        wire_num++;
      }
      else {
        new_line += " " + to_string(wires.find(token)->second);
      }
      str.erase(0, pos + 1);
  }

  if (wire) {
    wires.insert({str, wire_num});
    wire_num++;
  }
  else {
    new_line += " " + to_string(wires.find(str)->second);
    new_line.erase(new_line.begin());
    cout << new_line << " 0" << endl; // change to outfile
  }
}


// Time to reinvent that wheel, poorly!
void parser::parse_line(string& line, int& line_num) {
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
    break;
  case 'o':
    if (line[1] == 'u') { // output
        cout << ("c " + line) << endl;
    }
    else { // or
      // (~x + z)(~y + z)(x + y + ~z)
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 3);
      line.pop_back();
      line.pop_back();
      get_tokens(line, 3);
    }
    break;
  case 'r': // reg
    cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      get_tokens(line, 0, true);
    break;
  case 'w': // wire
    cout << ("c " + line) << endl;
    line.erase(line.begin(), line.begin() + 5);
    line.pop_back();
    get_tokens(line, 0, true);
    break;
  case 'a':
    if (line[1] == 'n') { // and
      // (x + ~z)(y + ~z)(~x + ~y + z)
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      line.pop_back();
      get_tokens(line, 3);
    }
      // We can ignore always tags
    break;
  case 'n':
    if (line[1] == 'a') { // nand
      // (x + z)(y + z)(~x + ~y + ~z)
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 5);
      line.pop_back();
      line.pop_back();
      get_tokens(line, 3);
    }
    else { 
      if(line[2] == 't') { // not
        // (~a v ~x)(a v x)
        cout << ("c " + line) << endl;
        line.erase(line.begin(), line.begin() + 4);
        line.pop_back();
        line.pop_back();
        get_tokens(line, 3);
      }
      else { // nor
        // (~x + ~z)(~y + ~z)(x + y + z)
        cout << ("c " + line) << endl;
        line.erase(line.begin(), line.begin() + 4);
        line.pop_back();
        line.pop_back();
        get_tokens(line, 3);
      }
    }
    break;
  case 'x':
    if (line[1] == 'o') { // xor
      // (~x + y + z)(x + ~y + z)(~x + ~y + ~z)(x + y + ~z)
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      line.pop_back();
      get_tokens(line, 4);
    }
    else { // xnor
      // (~x + ~y + z)(~x + y + ~z)(x + ~y + ~z)(x + y +z)
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 5);
      line.pop_back();
      line.pop_back();
      get_tokens(line, 4);
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
  // Close dimacs file
  outfile.close();
}
