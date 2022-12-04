#include <boost/config.hpp>
#include "view.hpp"

namespace tictactoe {
struct BOOST_SYMBOL_VISIBLE view_compact final : View {
    void ShowBoard(Game &thisGame) const override;
    std::vector<int> CommandRefactor(std::string &command) override;
    bool CheckIfCommandCorrect(std::string &command) override;  // NOLINT
};

std::vector<int> view_compact::CommandRefactor(std::string &command) {
    EraseExtraSpaces(command);
    std::vector<std::string> ParsedStrings = ParseBySpace(command);
    if (ParsedStrings[0] == "-1" && ParsedStrings[1] == "-1") {
        return {-1, -1};  // Exit command
    }
    EraseSomeShit(ParsedStrings);
    std::vector<int> BakedCords;
    BakedCords.reserve(3);
    for (auto &string : ParsedStrings) {
        BakedCords.emplace_back(std::stoi(string));
    }
    return BakedCords;
}

void view_compact::ShowBoard(Game &thisGame) const {
    for (auto &row : thisGame.GameBoard()) {
        for (auto element : row) {
            if (element == Game::OptionalPlayer::O) {
                std::cout << "O";
            } else if (element == Game::OptionalPlayer::X) {
                std::cout << "X";
            } else if (element == Game::OptionalPlayer::NTH) {
                std::cout << ".";
            }
        }
        std::cout << '\n';
    }
}

bool view_compact::CheckIfCommandCorrect(std::string &command) {
    EraseExtraSpaces(command);
    std::vector<std::string> ParsedStrings = ParseBySpace(command);
    if (ParsedStrings.size() != 2) {
        return false;  // If got more than two arguments
    }
    if (ParsedStrings[0] == "-1" && ParsedStrings[1] == "-1") {
        return true;  // Exit command
    }
    // NOLINTBEGIN
    for (auto &word : ParsedStrings) {
        for (auto &litter : word) {
            if ((litter < 42) ||
                (litter > 57)) {  // cppcheck-suppress useStlAlgorithm
                return false;     // If some char isn't a digit
            }
        }
        if ((word.size() == 1) && ((word[0] < 48) || (word[0] > 57))) {
            return false;
        }
    }
    // NOLINTEND
    return true;
}

// NOLINTBEGIN

extern "C" BOOST_SYMBOL_EXPORT view_compact current_view;
view_compact current_view;

// NOLINTEND

}  // namespace tictactoe
