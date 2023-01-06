/**
 * @file Parser.hpp
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
#include <array>
#include <list> // list could be used as stack
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Module {

class Parser {
    enum Relation : int {
        nil     = -2,
        less    = -1,
        equal   = 0,
        greater = 1,
    };
    std::unordered_set<std::string> operators {
        "+", "-", "*", "/", "(", ")", "#"
    };
    std::unordered_map<std::string, int> token_index_map {
        {"+",  0},
        { "-", 1},
        { "*", 2},
        { "/", 3},
        { "(", 4},
        { ")", 5},
        { "#", 6},
    };

    std::vector<std::vector<Relation>> relation_table {
        {greater,  greater, less,    less,    less, greater, greater},
        { greater, greater, less,    less,    less, greater, greater},
        { greater, greater, greater, greater, less, greater, greater},
        { greater, greater, greater, greater, less, greater, greater},
        { less,    less,    less,    less,    less, equal,   nil    },
        { greater, greater, greater, greater, nil,  greater, greater},
        { less,    less,    less,    less,    less, nil,     equal  },
    };
    Relation operator()(const std::string& from, const std::string& to) {
        return relation_table[token_index_map[from]][token_index_map[to]];
    };

    std::vector<std::string> TokenStream;
    std::list<std::string>   OperatorStack;
    std::list<std::string>   PostOrderStack;

    using TokenStreamType = std::vector<std::string>;
    void receive_TokenStream(const TokenStreamType& received) {
        TokenStream = received;
    }
    void trans_into_PostOrder() {
        Parser& curr_parse_process = *this;
        OperatorStack.push_back("#");
        int token_idx = 1; // != 0 => because we could ignore first `#` here
        rend_two_stacks();
        while (!OperatorStack.empty() && token_idx < TokenStream.size()) {
            const std::string& curr_token = TokenStream[token_idx];
            if (!operators.contains(curr_token)) {
                // number, join the `PostOrderStack`
                PostOrderStack.push_back(curr_token);
                // update curr_token
                ++token_idx;
            } else {
                const std::string& TOP      = OperatorStack.back();
                const std::string& curr     = curr_token;
                Relation           relation = (curr_parse_process)(TOP, curr);
                if (relation == nil) {
                    OperatorRelationException(curr);
                } else if (relation == less) {
                    // top < curr
                    OperatorStack.push_back(curr);
                    // update curr_token
                    ++token_idx;
                } else if (relation == equal) {
                    // top == curr
                    OperatorStack.pop_back();
                    // update curr_token
                    ++token_idx;
                } else {
                    // top > curr
                    PostOrderStack.push_back(TOP);
                    OperatorStack.pop_back();
                }
            }
            rend_two_stacks();
        }
        if (!OperatorStack.empty()) {
            OperatorMatchException();
        }
    }

    void rend_two_stacks() {
        std::cout << "Current Operator Stack >> ";
        std::for_each(
            OperatorStack.begin(),
            OperatorStack.end(),
            [](const std::string& curr) {
                std::cout << curr << " ";
            }
        );
        std::cout << std::endl;
        std::cout << "Current Post Order Stack >> ";
        std::for_each(
            PostOrderStack.begin(),
            PostOrderStack.end(),
            [](const std::string& curr) {
                std::cout << curr << " ";
            }
        );
        std::cout << std::endl;
        std::cout << std::endl;
    }

    void OperatorRelationException(
        const std::string& curr
    ) {
        std::cout << OperatorStack.back()
                  << " and "
                  << curr
                  << " 's relation is nil, error occurs!"
                  << std::endl;
        throw std::runtime_error("Operator relation is nil, error occurs!");
    }
    void OperatorMatchException(
    ) {
        std::list<std::string> remained;
        std::for_each(
            OperatorStack.begin(),
            OperatorStack.end(),
            [&remained](const std::string& curr) {
                remained.push_back(curr);
            }
        );
        std::cout << "Operator Stack can't be cleared! Here's the remaining part:" << std::endl;
        std::cout << "\t";
        std::for_each(
            remained.begin(),
            remained.end(),
            [](const std::string& curr) {
                std::cout << curr << " ";
            }
        );
        std::cout << std::endl;
        std::cout << "One case is that you've input too many operators" << std::endl;
        std::cout << "Another one is that column in input CAN'T MATCH, such as \"(...(...)\" " << std::endl;
        throw std::runtime_error("Input <operator part> is not correct, semantic error occurs!");
    }

public:
    auto parser_process(const TokenStreamType& received) {
        std::cout << "=========== Parser ===========" << std::endl;
        std::cout << std::endl;

        receive_TokenStream(received);
        trans_into_PostOrder();

        std::cout << "Parser have successfully extracted `Post Order Stack`!" << std::endl;
        std::cout << std::endl;
        std::cout << "=========== End of Parser ===========" << std::endl;
        std::cout << std::endl;

        return PostOrderStack;
    }
};

} // namespace Module