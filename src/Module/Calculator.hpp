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

    using P_Stack
        = std::list<std::string>;
    using R_Stack = std::list<long double>;

    std::list<std::string> PostOrderStack;
    std::list<long double> DecimalStack;

    void decimal_operate(Operation opt) {
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
        for (const std::string& curr : PostOrderStack) {
            bool if_add = curr == "+";
            bool if_sub = curr == "-";
            bool if_mul = curr == "*";
            bool if_div = curr == "/";
            bool if_num = !if_add && !if_sub && !if_mul && !if_div;
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
        }
    }

public:
    void calculate_process(const P_Stack& in) {
        import_PostOrderStack(in);
        calculate();
        double long res = DecimalStack.back();
        std::cout << "Result = " << res << std::endl;
        std::cout << std::endl;
    }
};

} // namespace Module