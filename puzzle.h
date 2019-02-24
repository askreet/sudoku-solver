//
// Created by kyle on 2/9/19.
//

#ifndef SUDOK_NO_PUZZLE_H
#define SUDOK_NO_PUZZLE_H

#include "types.h"

using square_pred = const std::function<bool(const square&)>&;
using square_visitor = const std::function<void(square&)>&;

class sudoku_puzzle {
public:
    void for_each(square_pred pred, square_visitor func);

    void forEachSquare(square_visitor func);

    void forEachUnsolvedSquare(square_visitor func);

    void forEachSquareInRow(Row row, square_visitor func);

    void forEachSquareInColumn(Col column, square_visitor func);

    void forEachSquareInBlock(Block block, square_visitor func);

    int squaresInRowWithCandidate(Row row, int value);

    int squaresInColumnWithCandidate(Col col, int value);

    int squaresInBlockWithCandidate(Block block, int value);

    explicit sudoku_puzzle(std::array<int, 81> starting_puzzle);

    bool solved() const;

private:
    std::vector<square> squares = {};
};

#endif //SUDOK_NO_PUZZLE_H
