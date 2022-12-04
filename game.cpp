#include "game.hpp"
#include <iostream>

namespace tictactoe {
Game::Game()
    : game_board(std::vector<std::vector<OptionalPlayer>>(
          10,
          std::vector<OptionalPlayer>(10, OptionalPlayer::NTH)
      )) {
}

bool Game::CheckCorrectMove(int row, int column) const {
    return (
        ((row < 10) && (row >= 0)) && ((column < 10) && (column >= 0)) &&
        (game_board[row][column] == OptionalPlayer::NTH)
    );
}

void Game::Move(int row, int column) {
    std::cout << '\n';
    game_board[row][column] = player;
    ChangePlayer();
    m_count_moves++;
}

bool CheckStraight(
    int startDy,
    int startDx,
    int updateDy,
    int updateDx,
    Game *thisGame
) {
    int countInARow = 0;
    int maxCountInARow = 0;
    while ((startDx <= 9) && (startDy <= 9) && (startDx >= 0) && (startDy >= 0)
    ) {
        if (thisGame->GameBoard()[startDy][startDx] == thisGame->Player()) {
            countInARow++;
            maxCountInARow = std::max(countInARow, maxCountInARow);
        } else {
            countInARow = 0;
        }

        startDx += updateDx;
        startDy += updateDy;
    }
    return (maxCountInARow >= 5);
}

bool Game::CheckIfPlayerWin(int row, int column) {
    Game::ChangePlayer();
    bool CheckWin = false;
    if (CheckStraight(row, 0, 0, 1, this) ||
        CheckStraight(0, column, 1, 0, this)) {
        CheckWin = true;
    }
    int dy = (column + row < 9) ? (column + row) : 9;
    int dx = (column + row < 9) ? 0 : (column + row - 9);
    if (CheckStraight(dy, dx, -1, 1, this)) {
        CheckWin = true;
    }
    dy = (column > row) ? 0 : (row - column);
    dx = (column > row) ? (column - row) : 0;
    if (CheckStraight(dy, dx, 1, 1, this)) {
        CheckWin = true;
    }
    ChangePlayer();
    return CheckWin;
}

}  // namespace tictactoe