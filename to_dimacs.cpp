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
#include <parser.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace Convert;
using namespace std;

const string delimiters = "(,";
vector<string> prev_out;
vector<string> next_in;
unordered_map<string, int> wires;
int wire_num = 1;
extern int unroll_num;
vector<string> out;
int pvals[2];

void convert::get_dimacs(string& str, int gate) {
  size_t pos;
  string wire;
  vector<string> cnf;
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
        cnf.push_back(to_string(wires.find(wire + to_string(k))->second));
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
      cnf.push_back(to_string(wires.find(str + to_string(l))->second));
    }
    i++;
  }

  for (int j = 0; j < unroll_num; j++) {
    switch (gate) {
    case 1: // or
      // (~x + z)(~y + z)(x + y + ~z)
      out.push_back("-"+ cnf[2 + j]+ " "+ cnf[0 + j]+ " 0");
      out.push_back("-"+ cnf[4 + j]+ " "+ cnf[0 + j]+ " 0");
      out.push_back(cnf[2 + j]+ " "+ cnf[4 + j]+ " -"+ cnf[0 + j]+ " 0");
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
    case 8: // input
      // Initialize input states
      for (int m = 0; m < i - 1; m++) {
        if (cnf[m + j] != "") {
          if ((unroll_num == 1) || (!j && !(m % 2))) {
            out.push_back("-" + cnf[m] + " 0");
            pvals[1] += 1;
          }
          else if (prev_out.size() > m && (m % 2)) { // Output declared before input
            for (int h = 0; h < next_in.size()/2; h++) {
              out.push_back(prev_out[h] + " -" + cnf[m + j * 2] + " 0");
              out.push_back("-" + prev_out[h] + " " + cnf[m + j * 2] + " 0");
              pvals[1] += 2;
            }
          }
          else if (m % 2) {
            next_in.push_back(cnf[m + j * 2]);
          }
        }
      }
      break;
    case 9: // out
      // Initialize input states
      for (int m = 0; m < i - 1; m++) {
        if (cnf[m + j] != "") {
          if (j + 1 == unroll_num && !(m % 2)) {
            out.push_back(cnf[m]+ " 0");
            pvals[1] += 1;
          }
          else if ((next_in.size() > m) && (m % 2)) { // Input declared before output
            for (int h = 0; h < next_in.size()/2; h++) {
              out.push_back(cnf[m + j * 2] + " -" + next_in[h + j] + " 0");
              out.push_back("-" + cnf[m + j * 2] + " " + next_in[h + j] + " 0");
              pvals[1] += 2;
            }
          }
          else if (m % 2) {
            prev_out.push_back(cnf[m + j * 2]);
          }
        }
      }
      break;
    default: break;
    }
  }
  str = new_str;
  pvals[0] = wires.size();
}
