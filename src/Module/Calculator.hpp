/**
 * @file Calculator.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <algorithm>
#include <exception>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

namespace Module {

class Calculator {
    enum Operation : int {
        add      = 0,
        sub      = 1,
        multiply = 2,
        divide   = 3,
    };

    using P_Stack = std::list<std::string>;
    using R_Stack = std::list<long double>;

    std::list<std::string> PostOrderStack;
    std::list<long double> DecimalStack;

    void decimal_operate(Operation opt) {
        if (DecimalStack.size() < 2) {
            TooManyOperatorsException(opt);
        }
        long double right = DecimalStack.back();
        DecimalStack.pop_back();
        long double left = DecimalStack.back();
        DecimalStack.pop_back();
        switch (opt) {
        case add:
            DecimalStack.push_back(left + right);
            break;
        case sub:
            DecimalStack.push_back(left - right);
            break;
        case multiply:
            DecimalStack.push_back(left * right);
            break;
        case divide:
            DecimalStack.push_back(left / right);
            break;
        }
    }
    void import_PostOrderStack(const P_Stack& in) {
        PostOrderStack = in;
    }

    void calculate() {
        rend_two_stacks();
        while (!PostOrderStack.empty()) {
            const std::string& curr   = PostOrderStack.front();
            bool               if_add = curr == "+";
            bool               if_sub = curr == "-";
            bool               if_mul = curr == "*";
            bool               if_div = curr == "/";
            bool               if_num = !if_add && !if_sub && !if_mul && !if_div;
            if (if_num) {
                try {
                    long double to_push = std::stold(curr);
                    DecimalStack.push_back(to_push);
                } catch (const std::invalid_argument& e) {
                    std::cout << "Can't cast from string to long double, because:"
                              << e.what()
                              << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cout << "Can't cast from string to long double, because:"
                              << e.what()
                              << std::endl;
                }
            } else if (if_add) {
                decimal_operate(add);
            } else if (if_sub) {
                decimal_operate(sub);
            } else if (if_mul) {
                decimal_operate(multiply);
            } else {
                decimal_operate(divide);
            }
            PostOrderStack.pop_front();
            rend_two_stacks();
        }
    }

    void rend_two_stacks() {
        std::cout << "Current Post Order Stack >> ";
        std::for_each(
            PostOrderStack.begin(),
            PostOrderStack.end(),
            [](const std::string& curr) {
                std::cout << curr << " ";
            }
        );
        std::cout << std::endl;
        std::cout << "Current Decimal Stack >> ";
        std::for_each(
            DecimalStack.begin(),
            DecimalStack.end(),
            [](const long double& curr) {
                std::cout << curr << " ";
            }
        );
        std::cout << std::endl;
        std::cout << std::endl;
    }

    void TooManyOperatorsException(Operation opt) {
        auto get_optr = [opt]() {
            std::string ret;
            switch (opt) {
            case add:
                ret = "+";
                break;
            case sub:
                ret = "-";
                break;
            case multiply:
                ret = "*";
                break;
            case divide:
                ret = "/";
                break;
            }
            return ret;
        };
        const std::string& curr_operator = get_optr();
        std::cout << "Decimal Stack only contains >> "
                  << DecimalStack.back()
                  << std::endl;
        std::cout << "But, an operator ("
                  << curr_operator
                  << ") asks `TWO` decimal in the stack for calculation"
                  << std::endl;
        std::cout << "It means that, you've input TOO MANY Operators!"
                  << std::endl;
        throw std::logic_error("Too Many Operators!");
    }

public:
    void calculate_process(const P_Stack& in) {
        std::cout << "=========== Calculator ===========" << std::endl;
        std::cout << std::endl;

        import_PostOrderStack(in);
        calculate();

        std::cout << "Calculator have successfully get the result!" << std::endl;
        std::cout << std::endl;
        std::cout << "=========== End of Calculator ===========" << std::endl;
        std::cout << std::endl;

        double long res = DecimalStack.back();
        std::cout << "Result = " << res << std::endl;
        std::cout << std::endl;
    }
};

} // namespace Module