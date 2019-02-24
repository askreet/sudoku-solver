#ifndef SUDOK_NO_TYPES_H
#define SUDOK_NO_TYPES_H

#include <cstddef>
#include <array>
#include <cassert>
#include <set>

using Col = size_t;
using Row = size_t;

struct Location {
    Col col = 0;
    Row row = 0;

    bool operator==(const Location& other) const {
        return other.row == row && other.col == col;
    }
};

struct Block {
    size_t id = 0;

    constexpr std::array<Location, 9> locations() {
        assert(id >= 1 && id <= 9);

        // TODO: Learn more about matrix math or do something smarter here.
        Row firstRow = (((id - 1) / 3) * 3) + 1;
        Col firstCol = (((id - 1) % 3) * 3) + 1;

        return {
                Location{firstCol + 0, firstRow + 0},
                Location{firstCol + 1, firstRow + 0},
                Location{firstCol + 2, firstRow + 0},
                Location{firstCol + 0, firstRow + 1},
                Location{firstCol + 1, firstRow + 1},
                Location{firstCol + 2, firstRow + 1},
                Location{firstCol + 0, firstRow + 2},
                Location{firstCol + 1, firstRow + 2},
                Location{firstCol + 2, firstRow + 2},
        };
    }
};

class square {
public:
    void set(int8_t value) {
        solved_value = value;
        solved = true;
    }

    Row row() const { return _loc.row; }

    Col col() const { return _loc.col; }

    Location location() const { return _loc; }

    const std::set<int8_t>& candidates() const { return _candidates; }

    Block block() const { return _block; }

    void setLocation(Location loc) {
        _loc = loc;
        _block = Block{((_loc.col + 2) / 3) + (((_loc.row + 2) / 3) - 1) * 3};
    }

    bool is_solved() const { return solved; }

    bool could_be(int8_t value) const {
        if (solved) {
            return solved_value == value;
        } else {
            return _candidates.count(value) > 0;
        };
    }

    int value() const {
        return static_cast<int>(solved_value);
    }

    std::string stringDisplay() const {
        if (is_solved()) {
            return std::to_string(value());
        } else {
            std::string out;

            size_t ctr = 0;

            for (auto const& e : _candidates) {
                out += std::to_string(e);
                ctr++;

                if (ctr % 3 == 0 && ctr != _candidates.size()) {
                    out += "\n";
                } else {
                    out += " ";
                }
            }

            return out;
        }
    }

    bool operator==(square& other) {
        return other._loc == _loc;
    }

    void removeCandidate(int8_t value) {
        _candidates.erase(value);
        if (_candidates.size() == 1) {
            solved = true;
            for (auto& c : _candidates)
                solved_value = c;
        }
    }

private:
    std::set<int8_t> _candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    bool solved = false;
    int8_t solved_value = 0;
    Location _loc = {0, 0};
    Block _block = {0};
};

#endif //SUDOK_NO_TYPES_H
