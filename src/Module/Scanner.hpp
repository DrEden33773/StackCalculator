/**
 * @file Scanner.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../FileManager.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Module {

class Scanner {
    const std::filesystem::path& opt_src_path = FileManager::Filename::SourceFile;

    /* Move src to opt_path */
    std::filesystem::path src_file_path;
    bool                  if_need_to_move_src = true;

    /* Expression & TokenStream */
    std::string              Expression;
    std::vector<std::string> TokenStream;

    /* Tables */
    std::unordered_set<char> supported_char {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '+',
        '-',
        '*',
        '/',
        '(',
        ')',
        '#',
        '.',
    };
    std::unordered_set<char> operator_char {
        '+',
        '-',
        '*',
        '/',
        '(',
        ')',
    };
    std::unordered_set<char> signal_char {
        '#',
    };

    void locate_source_file() {
        bool if_opt_source_exist = std::filesystem::exists(opt_src_path);
        if (if_opt_source_exist) {
            bool if_opt_source_empty = std::filesystem::is_empty(opt_src_path);
            if (!if_opt_source_empty) {
                // should not ReLocate the src
                if_need_to_move_src = false;
                return;
            }
        }
        while (true) {
            std::cout << "Please input the `Path` of source file " << std::endl;
            std::cout << R"( E.G. => C:/Path/Name.txt | C:\\Path\\Name.txt )" << std::endl;
            std::cout << ">>> ";

            std::cin >> src_file_path;

            if (!std::filesystem::exists(src_file_path)) {
                std::cout << "Input path DO NOT EXIST! Please try again... " << std::endl;
                std::cout << std::endl;
                src_file_path.clear();
            } else {
                std::cout << std::endl;
                break;
            }
        }
    }
    void copy_the_source_file() {
        if (!if_need_to_move_src) {
            return;
        }
        FileManager::copy_source_file_from(src_file_path);
        std::cout << "Successfully get source file. It had been moved to: " << std::endl;
        std::cout << "\t" << opt_src_path << std::endl;
        std::cout << std::endl;
    }
    void scan_opt_file() {
        std::fstream opt_file;

        opt_file.open(opt_src_path, std::fstream::in);
        if (!opt_file.is_open()) {
            throw std::runtime_error("Failed to open `source.txt`!");
        }
        char curr_char = 0;
        while (opt_file.get(curr_char)) {
            if (curr_char == ' ' || curr_char == '\t' || curr_char == '\n') {
                continue;
            }
            Expression.push_back(curr_char);
        }
        opt_file.close();

        std::cout << "Scanning process is over! " << std::endl;
        std::cout << std::endl;
    }

public:
    void DecimalPointException() {
        throw std::runtime_error("Don't support `Float Calculation` yet!");
    }
    void SyntaxErrorException(const char& invalid = '?') {
        std::cout << "Entered `" << invalid << "` which caused syntax error!" << std::endl;
        throw std::runtime_error("Syntax error occurred!");
    }
    void StartEndSignalException() {
        throw std::runtime_error("`#` is not complete, can't calculate!");
    }
    void TooManySignalException() {
        throw std::runtime_error("Too many `#`, can't calculate!");
    }
    void EmptyException() {
        throw std::runtime_error("You haven't input any expression, semantic error occurs!");
    }
    void lexer() {
        if (Expression.front() != '#' || Expression.back() != '#') {
            StartEndSignalException();
        }
        int num_of_sig = 0;
        std::for_each(
            Expression.begin(),
            Expression.end(),
            [&num_of_sig](const char& curr) {
                if (curr == '#') {
                    ++num_of_sig;
                }
            }
        );
        if (Expression.size() == 2) {
            EmptyException();
        }
        if (num_of_sig > 2) {
            TooManySignalException();
        }
        std::string curr_token;
        for (const char& curr_char : Expression) {
            if (curr_char == ' ' || curr_char == '\t' || curr_char == '\n') {
                continue;
            }
            if (!supported_char.contains(curr_char)) {
                SyntaxErrorException(curr_char);
            }
            bool if_operator = operator_char.contains(curr_char);
            bool if_signal   = signal_char.contains(curr_char);
            if (if_operator || if_signal) {
                // curr_token ==move=> TokenStream
                if (curr_token != "") {
                    TokenStream.push_back(curr_token);
                }
                // clear curr_token
                curr_token.clear();
            }
            curr_token.push_back(curr_char);
            if (if_operator || if_signal) {
                // operator/signal ==move=> TokenStream
                if (curr_token != "") {
                    TokenStream.push_back(curr_token);
                }
                // clear operator/signal in curr_token
                curr_token.clear();
            }
        }
    }

    void rend_the_TokenStream() {
        std::cout << std::endl;
        std::cout << "Lexer have successfully extracted `TokenStream`..." << std::endl;
        std::cout << "Here's the `TokenStream` >>> " << std::endl;

        int token_idx = 0;
        std::for_each(
            TokenStream.begin(),
            TokenStream.end(),
            [&](const std::string& curr_token) {
                std::cout << "[" << token_idx << "] >> ";
                std::cout << curr_token << std::endl;
                ++token_idx;
            }
        );
        std::cout << std::endl;
    }

public:
    auto Debug_Keyboard_IO() {
        std::cout << "(Debug) Please input the expression" << std::endl;
        std::cout << ">>> ";
        std::getline(std::cin, Expression);
        lexer();
        rend_the_TokenStream();

        std::cout << "=========== End of Scanner ===========" << std::endl;
        std::cout << std::endl;

        return TokenStream;
    }
    auto File_IO() {
        locate_source_file();
        copy_the_source_file();
        scan_opt_file();
        lexer();
        rend_the_TokenStream();

        std::cout << "=========== End of Scanner ===========" << std::endl;
        std::cout << std::endl;

        return TokenStream;
    }
};

} // namespace Module