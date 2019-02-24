#include <string>
#include <cstddef>

#ifndef SUDOK_NO_ALGORITHM_H
#define SUDOK_NO_ALGORITHM_H

using std::string, std::vector;

const Location no_highlight = {0, 0};

struct step {
    size_t number;
    size_t pass;
    string description;
    string phase;
    sudoku_puzzle state;

    Location current = no_highlight;
    Location source = no_highlight;
};

class steps {
public:
    using iterator = std::list<step>::iterator;

    void start_pass();

    void start_phase(string _phase);

    void record_step(sudoku_puzzle puzzle, string description, square& current, square& source);

    void record_step(sudoku_puzzle puzzle, string description, square& current);

    iterator begin();

    iterator end();

    size_t size() { return count; }
private:
    size_t count = 0;
    size_t pass = 0;
    string phase = "Unknown";
    std::list<step> steps = {};
};

steps solve(sudoku_puzzle& puzzle);

#endif //SUDOK_NO_ALGORITHM_H
