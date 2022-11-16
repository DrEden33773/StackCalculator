/**
 * @file TaskManager.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "FileManager.hpp"
#include "Module/Calculator.hpp"
#include "Module/Parser.hpp"
#include "Module/Scanner.hpp"

#include <list>
#include <string>
#include <vector>

namespace TaskManager {

void Non_File() {
    Module::Scanner    scanner;
    Module::Parser     parser;
    Module::Calculator calculator;

    std::vector<std::string> TokenStream    = scanner.Debug_Keyboard_IO();
    std::list<std::string>   PostOrderStack = parser.parser_process(TokenStream);
    calculator.calculate_process(PostOrderStack);
}

} // namespace TaskManager
