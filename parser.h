#include <string>

#ifndef PARSER_H
#define PARSER_H

namespace Parse {
  class parser {
    public:
      void parse_line(std::string& line, int& line_num);
  };
}

#endif