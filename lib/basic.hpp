#ifndef BASIC_HPP
#define BASIC_HPP

#include <map>
#include <cctype>
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
        int result = 0, sg = +1;
        for (size_t p = str[0] == '-' ? sg = -1, 1: 0; p < str.size(); ++p) {
            result = result * 10 + str[p] - '0';
        }
        return sg * result;
    }

    bool is_digit(const std::string& str) {
        if (str == "-") return false;
        for (size_t p = str[0] == '-'; p < str.size(); ++p)
            if (!isdigit(str[p])) return false;
        return true;
    }

    bool is_var(const std::string& str) {
        if (str == "LET") return false;
        return isalpha(str[0]);
    }

    bool is_element(const std::string& str) {
        return is_var(str) || is_digit(str);
    }

    bool is_sign(const std::string& str) {
        return str == "+" || str == "-" || str == "*" || str == "/";
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
