//
// Created by kyle on 2/9/19.
//

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "puzzle.h"
#include "algorithm.h"

using std::string;

void steps::start_pass() { pass++; }

void steps::start_phase(string _phase) { phase = move(_phase); }

void steps::record_step(sudoku_puzzle puzzle, string description, square& current, square& source) {
    size_t n = count + 1;

    steps.push_back({n, pass, move(description), phase, puzzle, current.location(), source.location()});

    count++;
}

void steps::record_step(sudoku_puzzle puzzle, string description, square& current) {
    size_t n = count + 1;

    steps.push_back({n, pass, move(description), phase, puzzle, current.location(), no_highlight});

    count++;
}

steps::iterator steps::begin() {
    return steps.begin();
}

steps::iterator steps::end() {
    return steps.end();
}

void removeAlreadyExistingValuesByRow(sudoku_puzzle& puzzle, steps& steps, square& square);

void removeAlreadyExistingValuesByColumn(sudoku_puzzle& puzzle, steps& steps, square& square);

void removeAlreadyExistingValuesByBlock(sudoku_puzzle& puzzle, steps& steps, square& square);

steps solve(sudoku_puzzle& puzzle) {
    steps steps;

    while (!puzzle.solved()) {
        steps.start_pass();

        steps.start_phase("Remove Conflicts");
        puzzle.forEachUnsolvedSquare([&](square& square) {
            removeAlreadyExistingValuesByRow(puzzle, steps, square);

            removeAlreadyExistingValuesByColumn(puzzle, steps, square);

            removeAlreadyExistingValuesByBlock(puzzle, steps, square);
        });

        steps.start_phase("Accept Unique Candidates");
        puzzle.forEachUnsolvedSquare([&](square& square) {
            for (int8_t candidate : square.candidates()) {
                if (puzzle.squaresInRowWithCandidate(square.row(), candidate) == 1) {
                    square.set(candidate);

                    std::ostringstream o;

                    o << "Only potential " << square.value() << " in row.";

                    steps.record_step(puzzle, o.str(), square);
                    return;
                }

                if (puzzle.squaresInColumnWithCandidate(square.col(), candidate) == 1) {
                    square.set(candidate);

                    std::ostringstream o;

                    o << "Only potential " << square.value() << " in column.";

                    steps.record_step(puzzle, o.str(), square);
                    return;
                }

                if (puzzle.squaresInBlockWithCandidate(square.block(), candidate) == 1) {
                    square.set(candidate);

                    std::ostringstream o;

                    o << "Only potential " << square.value() << " in block.";

                    steps.record_step(puzzle, o.str(), square);
                    return;
                }
            }
        });
    }

    return steps;
}

void removeAlreadyExistingValuesByRow(sudoku_puzzle& puzzle, steps& steps, square& square) {
    puzzle.forEachSquareInRow(square.row(), [&](class square& other_square) {
        if (other_square == square) return;

        if (other_square.is_solved() && square.could_be(other_square.value())) {
            square.removeCandidate(other_square.value());

            std::ostringstream o;

            o << "Removing " << other_square.value() << ". Already exists in row.";

            steps.record_step(puzzle, o.str(), square, other_square);
        }
    });
}

void removeAlreadyExistingValuesByColumn(sudoku_puzzle& puzzle, steps& steps, square& square) {
    puzzle.forEachSquareInColumn(square.col(), [&](class square& other_square) {
        if (other_square == square) return;

        if (other_square.is_solved() && square.could_be(other_square.value())) {
            square.removeCandidate(other_square.value());

            std::ostringstream o;

            o << "Removing " << other_square.value() << ". Already exists in column.";

            steps.record_step(puzzle, o.str(), square, other_square);
        }
    });
}

void removeAlreadyExistingValuesByBlock(sudoku_puzzle& puzzle, steps& steps, square& square) {
    puzzle.forEachSquareInBlock(square.block(), [&](class square& other_square) {
        if (other_square == square) return;

        if (other_square.is_solved() && square.could_be(other_square.value())) {
            square.removeCandidate(other_square.value());

            std::ostringstream o;

            o << "Removing " << other_square.value() << ". Already exists in block.";

            steps.record_step(puzzle, o.str(), square, other_square);
        }
    });
}