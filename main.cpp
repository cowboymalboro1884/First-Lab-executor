#include <boost/dll/import.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <string>
#include "game.hpp"
#include "view.hpp"

#if BOOST_VERSION >= 107600
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import
#endif

int main(int argc, char *argv[]) {  // NOLINT
    // NEED FOR CLANG-TIDY
    if (argc > 2) {
        std::cout << "Excepted one argument, got>>" << argc - 1 << '\n';
        return 0;
    }

    std::string default_view(argv[1]);  // NOLINT
    boost::shared_ptr<tictactoe::View> current_view =
        boost_dll_import_symbol<tictactoe::View>(
            default_view, "current_view",
            boost::dll::load_mode::append_decorations
        );

    tictactoe::Game MyGame;
    std::string command;

    std::cout << '\n';
    current_view->ShowBoard(MyGame);
    std::cout << MyGame.PrintPlayer() << " move: ";

    while (std::getline(std::cin, command)) {
        if (command.substr(0, 5) == "view-") {
            current_view = boost_dll_import_symbol<tictactoe::View>(
                command, "current_view",
                boost::dll::load_mode::append_decorations
            );
            current_view->ShowBoard(MyGame);
            std::cout << MyGame.PrintPlayer() << " move: ";
            continue;
        }
        if (!current_view->CheckIfCommandCorrect(command)) {
            std::cout << "Bad move!\n";
        } else {
            std::vector<int> Coordinates =
                current_view->CommandRefactor(command);
            int row = Coordinates[0];     // A row
            int column = Coordinates[1];  // A column

            if (row == -1 && column == -1) {
                return 0;
            }

            if (MyGame.CheckCorrectMove(row, column)) {
                MyGame.Move(row, column);
                current_view->ShowBoard(MyGame);
                if (MyGame.CountOfMoves() == 100) {
                    std::cout << "Draw.\n";
                    return 0;
                }
                if (MyGame.CheckIfPlayerWin(row, column)) {
                    MyGame.ChangePlayer();
                    std::cout << MyGame.PrintPlayer() << " wins!\n";
                    return 0;
                }
            } else {
                std::cout << "Bad move!\n";
            }
        }

        std::cout << MyGame.PrintPlayer() << " move: ";
    }
    return 0;
}