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
#include <fstream>
#include <iostream>
#include <parser.h>
#include <string>
#include <unordered_map>

using namespace Convert;
using namespace std;

// Open new dimacs file for writing
ofstream outfile ("out.dimacs");

const string delimiters = "(,";
unordered_map<string, int> wires;
int wire_num = 1;
extern int unroll_num;

void convert::get_dimacs(string& str, int gate) {
  size_t pos;
  string wire;
  string cnf [6];
  int i = 0;
  
  if ((pos = str.find(delimiters[0])) != string::npos) {
    str.erase(0, pos + 1);
  }

  // Maintain string for additional runs
  string new_str = str;

  while ((pos = str.find(delimiters[1])) != string::npos) {
    wire = str.substr(0, pos);
    for (int k = 0; k < unroll_num; k++) {
      if (!gate) {
        wires.insert({wire + to_string(k), wire_num});
        wire_num++;
      }
      else {
        cnf[i] = to_string(wires.find(wire + to_string(k))->second);
      }
      i++;
    }
    str.erase(0, pos + 1); 
  }

  for (int l = 0; l < unroll_num; l++) {
    if (!gate) {
      wires.insert({str + to_string(l), wire_num});
      wire_num++;
    }
    else {
      cnf[i] = to_string(wires.find(str + to_string(l))->second);
    }
    i++;
  }

  for (int j = 0; j < unroll_num; j++) {
    switch (gate) {
    case 1: // or
      // (~x + z)(~y + z)(x + y + ~z)
      cout << "-" << cnf[2 + j] << " " << cnf[0 + j] << " 0" << endl;
      cout << "-" << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      cout << cnf[2 + j] << " " << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      break;
    case 2: // and
      // (x + ~z)(y + ~z)(~x + ~y + z)
      cout << cnf[2 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << "-" << cnf[2 + j] << " -" << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      break;
    case 3: // nand
      // (x + z)(y + z)(~x + ~y + ~z)
      cout << cnf[2 + j] << " " << cnf[0 + j] << " 0" << endl;
      cout << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      cout << "-" << cnf[2 + j] << " -" << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      break;
    case 4: // not
      // (~x v ~z)(x v z)
      cout << "-" << cnf[2 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << cnf[2 + j] << " " << cnf[0 + j] << " 0" << endl;
      break;
    case 5: //nor
      // (~x + ~z)(~y + ~z)(x + y + z)
      cout << "-" << cnf[2 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << "-" << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << cnf[2 + j] << " " << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      break;
    case 6: //xor
      // (~x + y + z)(x + ~y + z)(~x + ~y + ~z)(x + y + ~z)
      cout << "-" << cnf[2 + j] << " " << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      cout << cnf[2 + j] << " -" << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      cout << "-" << cnf[2 + j] << " -" << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << cnf[2 + j] << " " << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      break;
    case 7: // xnor
      // (~x + ~y + z)(~x + y + ~z)(x + ~y + ~z)(x + y + z)
      cout << "-" << cnf[2 + j] << " -" << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      cout << "-" << cnf[2 + j] << " " << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << cnf[2 + j] << " -" << cnf[4 + j] << " -" << cnf[0 + j] << " 0" << endl;
      cout << cnf[2 + j] << " " << cnf[4 + j] << " " << cnf[0 + j] << " 0" << endl;
      break;
    case 8: // input
      // Initialize input states
      for (int j = 0; j <= i; j++) {
        if (cnf[j] != "") {
          cout << cnf[j] << " 0" << endl;
        }
      }
      break;
    case 9: // output
      // Initialize input states
      for (int j = 0; j <= i; j++) {
        if (cnf[j] != "") {
          cout << cnf[j] << " 0" << endl;
        }
      }
      break;
    default:
      break;
    }
  }
  str = new_str;
  // Close dimacs file
  outfile.close();
}
