#include <boost/config.hpp>
#include "view.hpp"

namespace tictactoe {
struct BOOST_SYMBOL_VISIBLE view_pretty final : View {
    void ShowBoard(Game &thisGame) const override;
    std::vector<int> CommandRefactor(std::string &command) override;
    bool CheckIfCommandCorrect(std::string &command) override;
};

void PrintARow(int numOfRow, Game &thisGame, int line) {
    std::vector<std::vector<Game::OptionalPlayer>> board = thisGame.GameBoard();
    for (int column = 0; column < 10; ++column) {
        std::cout << "|";
        if (board[numOfRow][column] == Game::OptionalPlayer::NTH) {
            std::cout << "    ";
        }
        if (board[numOfRow][column] == Game::OptionalPlayer::X) {
            if (line % 3 == 1) {
                std::cout << R"( \/ )";
            }
            if (line % 3 == 2) {
                std::cout << R"( /\ )";
            }
        }
        if (board[numOfRow][column] == Game::OptionalPlayer::O) {
            std::cout << R"( @@ )";
        }
        if (column == 9) {
            std::cout << "|\n";
        }
    }
}

void view_pretty::ShowBoard(tictactoe::Game &thisGame) const {
    int numOfRow = 0;
    int line = 0;
    while (line < 30) {
        if (line % 3 == 0) {
            std::cout
                << "  +----+----+----+----+----+----+----+----+----+----+\n";
        }
        if (line % 3 == 1) {
            std::cout << 9 - numOfRow << " ";
            PrintARow(numOfRow, thisGame, line);
        }
        if (line % 3 == 2) {
            std::cout << "  ";
            PrintARow(numOfRow, thisGame, line);
            numOfRow++;
        }
        line++;
    }
    std::cout << "  +----+----+----+----+----+----+----+----+----+----+\n";
    std::cout << "    a    b    c    d    e    f    g    h    i    j   \n";
}

std::vector<int> view_pretty::CommandRefactor(std::string &command) {
    std::vector<int> BakedCords(2);
    BakedCords[1] = (command[0] - 49 - '0');  // СТОЛБЕЦ
    BakedCords[0] = 9 - (command[1] - '0');   // СТРОКА
    return BakedCords;
}

bool view_pretty::CheckIfCommandCorrect(std::string &command) {
    return (
        (command.size() == 2) && ((command[0] > 96) && (command[0] < 107)) &&
        ((command[1] > 47) && (command[1] < 58))
    );
}

// NOLINTBEGIN

extern "C" BOOST_SYMBOL_EXPORT view_pretty current_view;
view_pretty current_view;

// NOLINTEND

}  // namespace tictactoe