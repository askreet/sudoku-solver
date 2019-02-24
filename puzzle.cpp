//
// Created by kyle on 2/9/19.
//

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#include <set>
#include <list>
#include <iostream>
#include <functional>
#include <cassert>
#include <array>
#include "puzzle.h"
#include "types.h"

void sudoku_puzzle::for_each(square_pred pred, square_visitor func) {
    for (auto& s : squares)
        if (pred(s))
            func(s);
}

void sudoku_puzzle::forEachSquare(square_visitor func) {
    for_each([](const square&){ return true; }, func);
}

void sudoku_puzzle::forEachUnsolvedSquare(square_visitor func) {
    for (auto& s : squares) {
        if (!s.is_solved()) func(s);
    }
}

void sudoku_puzzle::forEachSquareInRow(Row row, square_visitor func) {
    assert(row >= 1 && row <= 9);

    for (Row i = 9 * (row - 1); i < 9 * row; i++)
        func(squares.at(i));
}

void sudoku_puzzle::forEachSquareInColumn(Col column, square_visitor func) {
    assert(column >= 1 && column <= 9);

    for (Col i = column - 1; i < 81; i += 9)
        func(squares.at(i));
}

void sudoku_puzzle::forEachSquareInBlock(Block block, square_visitor func) {
    for (Location loc : block.locations()) {
        size_t index = ((loc.col - 1) % 9) +
                       ((loc.row - 1) * 9);

        func(squares.at(index));
    }
}

int sudoku_puzzle::squaresInRowWithCandidate(Row row, int value) {
    int count = 0;

    forEachSquareInRow(row, [&](square& square) {
        if (square.could_be(value))
            count++;
    });

    return count;
}

int sudoku_puzzle::squaresInColumnWithCandidate(Col col, int value) {
    int count = 0;

    forEachSquareInColumn(col, [&](square& square) {
        if (square.could_be(value))
            count++;
    });

    return count;
}

int sudoku_puzzle::squaresInBlockWithCandidate(Block block, int value) {
    int count = 0;

    forEachSquareInBlock(block, [&](square& square) {
        if (square.could_be(value))
            count++;
    });

    return count;
}

sudoku_puzzle::sudoku_puzzle(std::array<int, 81> starting_puzzle)
        : squares(81) {
    for (Col i = 0; i < 81; i++) {
        if (starting_puzzle[i] != 0) {
            squares.at(i).set(starting_puzzle[i]);
        }

        squares.at(i).setLocation({(i % 9) + 1, (i / 9) + 1});
    }
}

bool sudoku_puzzle::solved() const {
    return all_of(squares.cbegin(), squares.cend(), [](const square& s) { return s.is_solved(); });
}

