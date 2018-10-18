#ifndef PROGRAM_HPP
#define PROGRAM_HPP

namespace basic{
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
      if (is_alpha(str))
        return to_int(str);
      ASSERT(map.count(str), "VARIABLE NOT DEFINED");
      return map[str];
    }
    
    int eval(const expr_iter& begin,
             const expr_iter& end) {
      int first = 0, second = eval(*begin), temp;
      for (expr_iter ptr = begin + 1; ptr != end; ++ptr) {
        switch ((*ptr)[0]) {
        case '+': first += second; second = +eval(*++ptr); break;
        case '-': first += second; second = -eval(*++ptr); break;
        case '*': second *= eval(*++ptr); break;
        case '/':
          temp = eval(*++ptr);
          ASSERT(temp, "DIVIDE BY ZERO");
          second /= temp;
        }
      }
      return first + second;
    }

    void print(const string& str) {
      std::cout << eval(str);
    }

    void input(const string& name) {
      std::cout << "?";

      std::string content;

      std::getline(std::cin, content);
      ASSERT(is_alpha(content), "INVALID NUMBER");
      // ASSERT(map.count(name), "VARIABLE NOT DEFINED");

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
    
    int execute(int line_number) {
      step& s = program[line_number];

      switch (s.type) {
      case 4: return -1;
      case 5:
        ASSERT(program.count(to_int(s.expr[2])), "LINE NUMBER ERROR");
        return to_int(s.expr[2]);
      case 1: assign(s.expr.begin() + 2, s.expr.end()); break;
      case 6:
        if (compare(s.expr.begin() + 2, s.expr.end() - 2))
          return to_int(*s.expr.rbegin());
        break;
      case 2: print(*(s.expr.begin() + 2)); break;
      case 3: input(*(s.expr.begin() + 2));
      }

      return line_number + 1;
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

    bool insert (const string& str) {
      step s;
      s.expr = format(str);

      if (!command_id.count(s.expr[1]))
        return false;
      
      s.type = command_id[s.expr[1]];

      /*
        {"REM", 0},
        {"LET", 1},
        {"PRINT", 2},
        {"INPUT", 3},
        {"END", 4},
        {"GOTO", 5},
        {"IF", 6}
      */
      // switch (s.type) {
      // case 1: 
      // }
      
      program[to_int(s.expr[0])] = s;
      return true;
    }
    
    void run() {
      int line_number = program.begin() -> first;
      while (program.count(line_number)) 
        line_number = execute(line_number);
    }
  };
}

#endif
