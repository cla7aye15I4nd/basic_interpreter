#ifndef PROGRAM_HPP
#define PROGRAM_HPP

namespace basic{
  class eDIVIDE_BY_ZERO{};
  class eINVALID_NUMBER{};
  class eVARIABLE_NOT_DEFINED{};
  class eLINE_NUMBER_ERROR{};
  
  static const eDIVIDE_BY_ZERO DIVIDE_BY_ZERO;
  static const eINVALID_NUMBER INVALID_NUMBER;
  static const eVARIABLE_NOT_DEFINED VARIABLE_NOT_DEFINED;
  static const eLINE_NUMBER_ERROR LINE_NUMBER_ERROR;
  
  static std::map<std::string, int> command_id = {
    {"REM", 0},
    {"LET", 1},
    {"PRINT", 2},
    {"INPUT", 3},
    {"END", 4},
    {"GOTO", 5},
    {"IF", 6}
  };
  
  class basic_interpreter{
    class step{
    public:
      int type;
      expression expr;
      
      step () = default;
      step (const step& s) = default;
      
      friend std::ostream& operator<< (std::ostream &sout, const step& s) {
        for (auto str : s.expr)
          sout << str << ' ';
        sout << std::endl;
        return sout;
      }
    };
    
    int eval(const string& str) {
      if (is_digit(str))
        return to_int(str); 
      ASSERT(map.count(str), VARIABLE_NOT_DEFINED);
      return map[str];
    }

    int eval(const expr_iter& begin,
             const expr_iter& end,
             const expr_iter& start,
             const std::vector<expr_iter>& match) {
      std::vector<int> atom;
      std::vector<char> oper;
      for (auto ptr = begin; ptr != end; ptr++) {
        char c = (*ptr)[0];
        if (c == '+' || c == '-' || c == '*' || c == '/')
          oper.emplace_back(c);
        else {
          if (c == '(') {
            atom.emplace_back(eval(ptr + 1, match[ptr - start], start, match));
            ptr = match[ptr - start];
          } else {
            atom.emplace_back(eval(*ptr));
          }
          while (!oper.empty() && oper.back() != '+' && oper.back() != '-') {
            char op = oper.back(); oper.pop_back();
            int y = atom.back(); atom.pop_back();
            int x = atom.back(); atom.pop_back();
            if (op == '*') atom.emplace_back(x * y);
            else if (op == '/') {
              ASSERT(y, DIVIDE_BY_ZERO);
              atom.emplace_back(x / y);
            }
          }
        }
      }
      while (!oper.empty()) {
        char op = oper.back(); oper.pop_back();
        int y = atom.back(); atom.pop_back();
        int x = atom.back(); atom.pop_back();
        if (op == '+') atom.emplace_back(x + y);
        else if (op == '-') atom.emplace_back(x - y);
      }
      return atom.back();
    }
    
    int eval(const expr_iter& begin,
             const expr_iter& end) {
      std::vector<expr_iter> match(end - begin), stack;
      for (expr_iter ptr = begin; ptr != end; ptr++) {
        if (*ptr == "(") stack.emplace_back(ptr);
        else if (*ptr == ")") {
          match[stack.back() - begin] = ptr;
          stack.pop_back();
        }
      }
      return eval(begin, end, begin, match);
    }

    void print(int digit) {
      std::cout << digit << std::endl;
    }

    void input(const string& name) {
      std::cout << "?\n";

      std::string content;
      
      std::getline(std::cin, content);
      ASSERT(is_digit(content), INVALID_NUMBER);
      
      map[name] = to_int(content);
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
      return 0;
    }
    
    variable_pool map;
    std::map<int, step> program;
    
    int execute(step s) {
      //step& s = program[line_number];

      switch (s.type) {
      case 4: return -1;
      case 5:
        ASSERT(program.count(to_int(s.expr[0])), LINE_NUMBER_ERROR);
        return to_int(s.expr[0]);
      case 1: assign(s.expr.begin(), s.expr.end()); break;
      case 6:
        if (compare(s.expr.begin(), s.expr.end() - 2))
          return to_int(*s.expr.rbegin());
        break;
      case 2: print(eval(s.expr.begin(), s.expr.end())); break;
      case 3: input(*(s.expr.begin()));
      }

      return 0;
    }
    
  public:
    void clear() {
      map.clear();
      program.clear();
    }

    void show() {
      for (auto it = program.begin(); it != program.end(); ++it)
        std::cout << it -> second;
    }

    bool check_expr(const expr_iter& begin,
                    const expr_iter& end) {
      int cnt = 0;
      for (auto ptr = begin; ptr != end; ptr++) {
        if (*ptr == "(") {
          ++cnt;
          if (ptr + 1 == end) return false;
          if (*(ptr + 1) == ")") return false;
        }
        if (*ptr == ")") --cnt;
        if ((is_var(*ptr) || is_digit(*ptr)) &&
            ptr + 1 != end &&
            (is_var(*(ptr + 1)) || is_digit(*(ptr + 1)))) return false;
      }
      
      return cnt == 0;
    }
    
#define REPORT(var) if (var) return false;
    bool check(step s) {
      switch (s.type) {
      case 1:
        REPORT(s.expr.size() < 3);
        REPORT(!is_var(s.expr[0]));
        REPORT(s.expr[1] != "=");
        REPORT(!check_expr(s.expr.begin() + 2, s.expr.end()));
        break;
      case 2:
        REPORT(s.expr.size() < 1);
        REPORT(!check_expr(s.expr.begin(), s.expr.end()));
        break;
      case 3:
        REPORT(s.expr.size() != 1);
        REPORT(!check_expr(s.expr.begin(), s.expr.end()));
        break;
      case 4:
        REPORT(s.expr.size() != 0);
        break;
      case 5:
        REPORT(s.expr.size() != 1);
        REPORT(!is_digit(s.expr[0]));
        break;
      case 6:
        REPORT(s.expr.size() < 5);
        REPORT(!is_digit(*(s.expr.end() - 1)));
        REPORT(*(s.expr.end() - 2) != "THEN");
        REPORT(!check_expr(s.expr.begin(), s.expr.end() - 2));
      }

      return true;
    }
    
    bool insert (const expression& expr) {
      step s;
      int id = to_int(expr[0]);
      s.expr = expr;
      if (!command_id.count(s.expr[1]))
        return false;
      
      s.type = command_id[s.expr[1]];
      s.expr.erase(s.expr.begin());
      s.expr.erase(s.expr.begin());

      if (!check(s)) return false;
      
      program[id] = s;
      return true;
    }

    bool append(const expression& expr) {
      step s;
      s.expr = expr;

      if (!command_id.count(s.expr[0]))
        return false;
      
      s.type = command_id[s.expr[0]];
      s.expr.erase(s.expr.begin());
      if (!check(s)) return false;

      int id = program.empty() ? 0 : (program.rbegin() -> first);
      program[id] = s;
      try {
        execute(program[id]);
      } catch(eDIVIDE_BY_ZERO) {
        std::cout << "DIVIDE BY ZERO" << std::endl;
      } catch(eINVALID_NUMBER) {
        std::cout << "INVALID NUMBER" << std::endl;
      } catch(eVARIABLE_NOT_DEFINED) {
        std::cout << "VARIABLE NOT DEFINED" << std::endl;
      } catch(eLINE_NUMBER_ERROR) {
        std::cout << "LINE NUMBER ERROR" << std::endl;
      }
      return true;
    }
    
    void run() {
      try {
        auto ptr = program.begin();
        while (ptr != program.end()) {
          int ret = execute(ptr -> second);
          switch (ret) {
          case -1: return;
          case  0: ptr++; break;
          default: ptr = program.find(ret);
          }
        }
      } catch(eDIVIDE_BY_ZERO) {
        std::cout << "DIVIDE BY ZERO" << std::endl;
      } catch(eINVALID_NUMBER) {
        std::cout << "INVALID NUMBER" << std::endl;
      } catch(eVARIABLE_NOT_DEFINED) {
        std::cout << "VARIABLE NOT DEFINED" << std::endl;
      } catch(eLINE_NUMBER_ERROR) {
        std::cout << "LINE NUMBER ERROR" << std::endl;
      }
    }
  };
}

#endif
