#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "basic.hpp"
#include "program.hpp"

namespace basic{
  class terminal{
    basic_interpreter code, feedback;
    
  public:
    bool run() {
      string command;
      while (true) {
        std::cout << "(basic):";
        std::getline(std::cin, command);

        if (command == "QUIT") {
          std::cout << "exit" << std::endl;
          return 0;
        }

        if (command == "HELP") {
          std::cout << "Minimal BASIC Interpreter";
        } else if (command == "CLEAR") {
          code.clear();
          feedback.clear();
        } else if (command == "RUN") {
          code.run();
        } else if (command == "LIST") {
          code.show();
        } else{
          expression expr = format(command);
          if (is_digit(expr[0])) {
            if (!code.insert(expr))
              std::cout << "Invalid Command";
          } else {
            if (!feedback.append(expr))
              std::cout << "Invalid Command";
          }
        }

        std::cout << std::endl;
      }
    }
  };
}

#endif
