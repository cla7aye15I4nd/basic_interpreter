#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "basic.hpp"
#include "program.hpp"

namespace basic{
  class terminal{
    basic_interpreter interpreter;

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
        } else if (command == "RUN") {
          interpreter.run();
        } else if (command == "LIST") {
          interpreter.show();
        } else{
          if (!interpreter.insert(command))
            std::cout << "Invalid Command";
        }

        std::cout << std::endl;
      }
    }
  };
}

#endif
