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

void get_dimacs(string& str, int gate) {
  size_t pos;
  string wire;
  string cnf [3];
  int i = 0;
  string new_str;
  
  if ((pos = str.find(delimiters[0])) != string::npos) {
    str.erase(0, pos + 1);
  }

  while ((pos = str.find(delimiters[1])) != string::npos) {
      wire = str.substr(0, pos);
      if (!gate) {
        wires.insert({wire, wire_num});
        wire_num++;
      }
      else {
        cnf[i] = to_string(wires.find(wire)->second);
      }
      str.erase(0, pos + 1);
      i++;
  }

  if (!gate) {
    wires.insert({str, wire_num});
    wire_num++;
  }
  else {
    cnf[i] = to_string(wires.find(str)->second);
  }

  switch (gate) {
  case 1: // or
    // (~x + z)(~y + z)(x + y + ~z)
    cout << "-" << cnf[1] << " " << cnf[0] << " 0" << endl;
    cout << "-" << cnf[2] << " " << cnf[0] << " 0" << endl;
    cout << cnf[1] << " " << cnf[2] << " -" << cnf[0] << " 0" << endl;
    break;
  case 2: // and
    // (x + ~z)(y + ~z)(~x + ~y + z)
    cout << cnf[1] << " -" << cnf[0] << " 0" << endl;
    cout << cnf[2] << " -" << cnf[0] << " 0" << endl;
    cout << "-" << cnf[1] << " -" << cnf[2] << " " << cnf[0] << " 0" << endl;
    break;
  case 3: // nand
    // (x + z)(y + z)(~x + ~y + ~z)
    cout << cnf[1] << " " << cnf[0] << " 0" << endl;
    cout << cnf[2] << " " << cnf[0] << " 0" << endl;
    cout << "-" << cnf[1] << " -" << cnf[2] << " -" << cnf[0] << " 0" << endl;
    break;
  case 4: // not
    // (~x v ~z)(x v z)
    cout << "-" << cnf[1] << " -" << cnf[0] << " 0" << endl;
    cout << cnf[1] << " " << cnf[0] << " 0" << endl;
    break;
  case 5: //nor
    // (~x + ~z)(~y + ~z)(x + y + z)
    cout << "-" << cnf[1] << " -" << cnf[0] << " 0" << endl;
    cout << "-" << cnf[2] << " -" << cnf[0] << " 0" << endl;
    cout << cnf[1] << " " << cnf[2] << " " << cnf[0] << " 0" << endl;
    break;
  case 6: //xor
    // (~x + y + z)(x + ~y + z)(~x + ~y + ~z)(x + y + ~z)
    cout << "-" << cnf[1] << " " << cnf[2] << " " << cnf[0] << " 0" << endl;
    cout << cnf[1] << " -" << cnf[2] << " " << cnf[0] << " 0" << endl;
    cout << "-" << cnf[1] << " -" << cnf[2] << " -" << cnf[0] << " 0" << endl;
    cout << cnf[1] << " " << cnf[2] << " -" << cnf[0] << " 0" << endl;
    break;
  case 7: // xnor
    // (~x + ~y + z)(~x + y + ~z)(x + ~y + ~z)(x + y + z)
    cout << "-" << cnf[1] << " -" << cnf[2] << " " << cnf[0] << " 0" << endl;
    cout << "-" << cnf[1] << " " << cnf[2] << " -" << cnf[0] << " 0" << endl;
    cout << cnf[1] << " -" << cnf[2] << " -" << cnf[0] << " 0" << endl;
    cout << cnf[1] << " " << cnf[2] << " " << cnf[0] << " 0" << endl;
    break;
  case 8: // input
    // Initialize input states
    for (int j = 0; j <= i; j++) {
      cout << cnf[j] << " 0" << endl;
    }
  default:
    break;
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
    line.erase(line.begin(), line.begin() + 6);
    line.pop_back();
    get_dimacs(line, 0);
    get_dimacs(line, 8);
    break;
  case 'o':
    if (line[1] == 'u') { // output
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 7);
      line.pop_back();
      get_dimacs(line, 0);
    }
    else { // or
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 3);
      line.pop_back();
      line.pop_back();
      get_dimacs(line, 1);
    }
    break;
  case 'r': // reg
    cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      get_dimacs(line, 0);
    break;
  case 'w': // wire
    cout << ("c " + line) << endl;
    line.erase(line.begin(), line.begin() + 5);
    line.pop_back();
    get_dimacs(line, 0);
    break;
  case 'a':
    if (line[1] == 'n') { // and
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      line.pop_back();
      get_dimacs(line, 2);
    }
      // We can ignore always tags
    break;
  case 'n':
    if (line[1] == 'a') { // nand
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 5);
      line.pop_back();
      line.pop_back();
      get_dimacs(line, 3);
    }
    else { 
      if(line[2] == 't') { // not
        cout << ("c " + line) << endl;
        line.erase(line.begin(), line.begin() + 4);
        line.pop_back();
        line.pop_back();
        get_dimacs(line, 4);
      }
      else { // nor
        cout << ("c " + line) << endl;
        line.erase(line.begin(), line.begin() + 4);
        line.pop_back();
        line.pop_back();
        get_dimacs(line, 5);
      }
    }
    break;
  case 'x':
    if (line[1] == 'o') { // xor
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 4);
      line.pop_back();
      line.pop_back();
      get_dimacs(line, 6);
    }
    else { // xnor
      cout << ("c " + line) << endl;
      line.erase(line.begin(), line.begin() + 5);
      line.pop_back();
      line.pop_back();
      get_dimacs(line, 7);
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
