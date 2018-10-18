#ifndef BASIC_HPP
#define BASIC_HPP

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_map>

#define ASSERT(expr, message) if (!(expr)) { throw message; }
namespace basic{
  using string = std::string;
  using variable_pool = std::unordered_map<string, int>;
  using expression = std::vector<string>;
  using expr_iter = expression::iterator;

  int to_int(const string& str) {
    int result = 0;
    for (auto ch : str)
      if (!isspace(ch))
        result = result * 10 + ch - '0';
    return result;
  }

  bool is_alpha(const std::string& str) {
    for (auto ch : str)
      if (!isalpha(ch) && !isspace(ch))
        return false;
    return true;
  }

  expression format(const string& str) {
    expression result;
    string element;
    
    for (auto ptr = str.begin(); ptr != str.end(); ) {
      if (isspace(*ptr)) {
        ptr++;
        continue;
      }
      element = "";
      if (isalpha(*ptr)) {
        while (isalpha(*ptr))
          element += *ptr++;
      } else if (isdigit(*ptr)) {
        while (isdigit(*ptr))
          element += *ptr++;
      } else{
        element += *ptr++;
      }
      result.push_back(element);
    }

    return result;
  }
}

#endif
