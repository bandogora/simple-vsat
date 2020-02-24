#include <parser.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void to_dimacs(int opp, int& line_num) {
    switch(opp) {
      case 1: // do a thing
        break;
      case 2: // do a thing
        break;
      case 3: // do a thing
        break;
      case 4: // do a thing
        break;
      case 5: // do a thing
        break;
      case 6: // do a thing
        break;
      case 7: // do a thing
        break;
      case 8: // do a thing
        break;
      case 9: // do a thing
        break;
      case 10: // do a thing
        break;
      case 11: // do a thing
        break;
      case 12: // do a thing
        break;
      case 13: // do a thing
        break;
      default: cerr << strerror(1) << "Unrecognized symbol on line " << line_num << endl;
    }
}

// (or|nor|and|nand|not|xor|xnor)
// Time to reinvent that wheel, poorly!
int parse_line(string& line, int& line_num) {
  if (line[0] == 'm') {
    to_dimacs(1, line_num); // module
  }
  else if (line[0] == 'i') {
    to_dimacs(2, line_num); // input
  }
  else if (line[0] == 'o') {
      if (line[1] == 'u') {
        to_dimacs(3, line_num); // output
      }
      else {
        to_dimacs(4, line_num); // or
      }
  }
  else if (line[0] == 'r') {
    to_dimacs(5, line_num); // reg
  }
  else if (line[0] == 'w') {
    to_dimacs(6, line_num); // wire
  }
  else if (line[0] == 'a') {
    if (line[1] == 'n') {
      to_dimacs(7, line_num); // and
    }
    else if (line[1] == 'l' ) {
      to_dimacs(8, line_num); // always
    }
  }
  else if (line[0] == 'n') {
    if (line[1] == 'a') {
      to_dimacs(9, line_num); // nand
    }
    else { 
      if(line[2] == 't') {
        to_dimacs(10, line_num); // not
      }
      else {
        to_dimacs(11, line_num); // nor
      }
    }
  }
  else if (line[0] == 'x') {
    if (line[1] == 'o') {
      to_dimacs(12, line_num); // xor
    }
    else {
      to_dimacs(13, line_num); // xnor
    }
  }
  else {
    cerr << strerror(1) << "Unrecognized symbol on line " << line_num << endl;
  }
}


