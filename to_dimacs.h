#include <string>

#ifndef TO_DIMACS_H
#define TO_DIMACS_H

namespace Convert {
  class convert {
    public:
      void get_dimacs(std::string& str, int gate);
  };
}

#endif