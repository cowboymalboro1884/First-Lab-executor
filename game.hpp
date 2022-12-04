#ifndef GAME_HPP_
#define GAME_HPP_

#include <boost/dll/import.hpp>
#include <boost/version.hpp>
#include <string>
#include <vector>
#include "game_fwd.hpp"

namespace tictactoe {
struct Game final {
    enum class OptionalPlayer { X, O, NTH };
    ~Game() = default;
    void Move(int row, int column);

    [[nodiscard]] bool CheckIfPlayerWin(int row, int column);

    [[nodiscard]] std::string PrintPlayer() const {
        return player == OptionalPlayer::O ? "O" : "X";
    }

    [[nodiscard]] OptionalPlayer Player() const {
        return player;
    }

    [[nodiscard]] int CountOfMoves() const {
        return m_count_moves;
    }

    [[nodiscard]] bool CheckCorrectMove(int row, int column) const;

    void ChangePlayer() {
        player =
            (player == OptionalPlayer::O ? OptionalPlayer::X : OptionalPlayer::O
            );
    }

    std::vector<std::vector<OptionalPlayer>> GameBoard() {
        return game_board;
    }

    Game(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(const Game &) = delete;
    Game &operator=(Game &&) = delete;
    explicit Game();

private:
    std::vector<std::vector<OptionalPlayer>> game_board;
    OptionalPlayer player = OptionalPlayer::O;
    int m_count_moves = 0;
};

}  // namespace tictactoe

#endif  // GAME_HPP_