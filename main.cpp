#include <bits/stdc++.h>

static const std::map<std::string, int> oper_pri = {
  {"*" , 0}, {"/" , 0},
  {"+" , 1}, {"-" , 1},
};

class basic_interpreter{
  using string = std::string;
  using variable_pool = std::unordered_map<string, int>;
  using expression = std::vector<string>;
  using expr_iter = expression::iterator;

  int to_int(const string& str) const{
    int result = 0;
    for (auto ch : str)
      result = result * 10 + ch - '0';
    return result;
  }
  int eval(const string& str) {
    if (isalpha(*str.begin()))
      return map[str];
    return to_int(str);
  }
  int eval(const expr_iter& begin,
           const expr_iter& end) {
    int first = 0, second = eval(*begin), oper = +1;
    for (expr_iter ptr = begin + 1; ptr != end; ++ptr) {
      if (*ptr == "*") second *= eval(*++ptr);
      else if (*ptr == "/") second /= eval(*++ptr);
      else if (*ptr == "+") { first = second; second = eval(*++ptr); oper = +1; }
      else if (*ptr == "-") { first = second; second = eval(*++ptr); oper = -1; }
    }
    return first + oper * second;
  }

  void assign(const expr_iter& begin,
              const expr_iter& end) {
    map[*begin] = eval(begin + 2, end);
  }

  bool compare(const expr_iter& begin,
               const expr_iter& end) {
    for (expr_iter ptr = begin + 1; ptr != end; ++ptr) {
      if (*ptr == ">") return eval(begin, ptr) >  eval(ptr + 1, end);
      if (*ptr == "<") return eval(begin, ptr) <  eval(ptr + 1, end);
      if (*ptr == "=") return eval(begin, ptr) == eval(ptr + 1, end);
    }
  }

  expression format(const string& str) {
    expression result;
    string element;
    
    for (auto ptr = str.begin(); ptr != str.end(); ) {
      if (isspace(*ptr++)) continue;
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

  class step{
    int type;
    expression expr;
  };

  variable_pool map;
  std::map<int, step> program;

  void clear() {
    map.clear();
    program.clear();
  }
    
public:
};

int main() {
  return 0;
}
