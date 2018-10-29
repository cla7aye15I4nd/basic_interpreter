#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "basic.hpp"
#include "program.hpp"

namespace basic{
    class terminal{
        basic_interpreter code;
    
    public:
        bool run() {
            string command;
            while (true) {
                //std::cerr << "(basic):";
                std::getline(std::cin, command);

                if (command == "QUIT") {
                    //std::cerr << "exit" << std::endl;
                    return 0;
                }

                if (command == "HELP") {
                    std::cout << "Minimal BASIC Interpreter";
                } else if (command == "CLEAR") {
                    code.clear();
                } else if (command == "RUN") {
                    code.run();
                } else if (command == "LIST") {
                    code.show();
                } else{
                    expression expr = format(command);
                    if (is_digit(expr[0])) {
                        if (!code.insert(expr))
                            std::cout << "SYNTAX ERROR" << std::endl;
                    } else {
                        if (!code.feedback(expr))
                            std::cout << "SYNTAX ERROR" << std::endl;
                    }
                }
            }
        }
    };
}

#endif
