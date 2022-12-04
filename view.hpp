#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <algorithm>
#include <boost/config.hpp>
#include <iostream>
#include <sstream>
#include "game.hpp"

namespace tictactoe {
struct BOOST_SYMBOL_VISIBLE View {
    virtual ~View() = default;
    virtual void ShowBoard(Game &thisGame) const = 0;

    virtual std::vector<int> CommandRefactor(std::string &command) = 0;
    virtual bool CheckIfCommandCorrect(std::string &command) = 0;

    View(const View &) = delete;
    View(View &&) = delete;
    View &operator=(const View &) = delete;
    View &operator=(View &&) = delete;

protected:
    View() = default;
};

inline void EraseExtraSpaces(std::string &command) {
    int k = 1;
    int f = 0;
    std::string command_new;
    for (char i : command) {
        if ((i != 32) && (i != 9)) {
            if ((k == 0) && (f == 1)) {
                command_new.push_back(' ');
            }
            command_new.push_back(i);
            k = 1;
            f = 1;
        } else {
            k = 0;
        }
    }

    command = command_new;
}

std::vector<std::string> inline ParseBySpace(std::string &command) {
    std::istringstream s_command(command);
    std::string string;
    std::vector<std::string> ParsedStrings;
    while (std::getline(s_command, string, ' ')) {
        ParsedStrings.push_back(string);
    }
    return ParsedStrings;
}

inline void EraseSomeShit(std::vector<std::string> &ParsedStrings) {
    int i = 0;
    for (auto word : ParsedStrings) {
        std::reverse(word.begin(), word.end());
        while ((word.back() == '0') || (word.back() == '-')) {
            if (word.size() == 1) {
                break;
            }
            word.pop_back();
        }
        std::reverse(word.begin(), word.end());
        ParsedStrings[i] = word;
        i++;
    }
}

}  // namespace tictactoe

#endif  // VIEW_HPP_