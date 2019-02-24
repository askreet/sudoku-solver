#include <array>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <sstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <benchmark/benchmark.h>

#include "types.h"
#include "puzzle.h"
#include "algorithm.h"

sf::Vector2f center_of(sf::FloatRect& rect) {
    return {
            rect.left + (rect.width / 2.0f),
            rect.top + (rect.height / 2.0f)
    };
}

sf::Vector2f center_of(sf::Text& text) {
    sf::FloatRect rect = text.getLocalBounds();

    return center_of(rect);
}

sf::Color color(const step& step, const square& square) {
    if (step.current == square.location()) {
        return sf::Color::Cyan;
    } else if (step.source == square.location()) {
        return sf::Color::Yellow;
    } else {
        return sf::Color::Transparent;
    }
}

class sudoku_puzzle_view {
public:
    sudoku_puzzle_view() {
        font.loadFromFile("fonts/arial.ttf");
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
    }

    void draw(sf::RenderWindow& window, step& step, size_t totalSteps) {
        sf::Vector2f square_dimensions = {
                (static_cast<float>(window.getSize().y) - ((2 * window_padding) + bottom_padding)) / 9.0f,
                (static_cast<float>(window.getSize().x) - (2 * window_padding)) / 9.0f
        };

        draw_blocks(window, square_dimensions);
        draw_squares(window, step, square_dimensions);

        std::ostringstream o;
        o << "Step " << step.number << " of " << totalSteps
          << " (" << step.phase << ", Pass " << step.pass << ")" << std::endl;
        o << step.description << std::endl;
        o << "Use N / P to step through algorithm." << std::endl;

        text.setString(o.str());
        text.setOrigin(0.0f, 0.0f);
        text.setPosition(window_padding, window.getSize().y - bottom_padding);
        window.draw(text);
    }

    sf::FloatRect square_rect_for(sf::Vector2f size, Row row, Col col) const {
        return {
                (col - 1) * size.x + window_padding,
                (row - 1) * size.y + window_padding,
                size.x,
                size.y
        };
    }

private:
    void draw_blocks(sf::RenderWindow& window, sf::Vector2f square_dimensions) {
        sf::RectangleShape rect;
        rect.setOutlineColor(sf::Color::Black);
        rect.setFillColor(sf::Color::Transparent);

        // Blocklines
        rect.setOutlineThickness(4.0f);
        rect.setSize(square_dimensions * 3.0f);

        for (Col col = 1; col <= 3; col++) {
            for (Row row = 1; row <= 3; row++) {
                sf::FloatRect squareRect = square_rect_for(square_dimensions * 3.0f, row, col);
                rect.setPosition({squareRect.left, squareRect.top});
                window.draw(rect);
            }
        }
    }

    void draw_squares(sf::RenderWindow& window, step& step, sf::Vector2f square_dimensions) {
        sf::RectangleShape rect;
        rect.setOutlineColor(sf::Color::Black);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(2.0f);
        rect.setSize(square_dimensions);

        step.state.forEachSquare([&](const square& square) {
            sf::FloatRect squareRect = square_rect_for(square_dimensions, square.row(), square.col());

            rect.setPosition({squareRect.left, squareRect.top});
            rect.setFillColor(color(step, square));

            window.draw(rect);

            text.setString(square.stringDisplay());
            text.setFillColor(square.is_solved() ? sf::Color::Black : sf::Color::Red);
            text.setOrigin(center_of(text));
            text.setPosition(center_of(squareRect));

            window.draw(text);
        });
    }

    sf::Font font = {};
    sf::Text text = {};
    float window_padding = 45.0f;
    float bottom_padding = 100.0f;
};

int main() {
    sudoku_puzzle puzzle = sudoku_puzzle({
                                                 3, 0, 0, 0, 0, 0, 2, 4, 0,
                                                 4, 8, 0, 2, 0, 0, 3, 0, 5,
                                                 0, 0, 0, 6, 4, 0, 0, 9, 0,
                                                 0, 4, 0, 8, 0, 7, 6, 0, 0,
                                                 0, 0, 0, 0, 3, 0, 0, 0, 0,
                                                 0, 0, 3, 1, 0, 4, 0, 5, 0,
                                                 0, 3, 0, 0, 6, 9, 0, 0, 0,
                                                 9, 0, 2, 0, 0, 5, 0, 1, 3,
                                                 0, 7, 5, 0, 0, 0, 0, 0, 4,
                                         });

    sf::RenderWindow window(sf::VideoMode(900, 1000), "Sudoku Solver Visualization");
    sudoku_puzzle_view puzzle_view;
    steps steps = solve(puzzle);
    auto step = steps.begin();
    auto last_step = std::prev(steps.end());
    sf::Event event{};

    std::cout << sizeof(std::ostream) << std::endl;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Key::Escape:
                        window.close();
                        break;

                    case sf::Keyboard::Key::N:
                        if (step != last_step)
                            step++;
                        break;

                    case sf::Keyboard::Key::P:
                        if (step != steps.begin())
                            step--;
                        break;

                    case sf::Keyboard::Key::B:
                        step = steps.begin();
                        break;

                    case sf::Keyboard::Key::E:
                        step = last_step;
                        break;

                    default:
                        break;
                }
            }
        }

        window.clear(sf::Color::White);
        puzzle_view.draw(window, *step, steps.size());

        window.display();
    }

    return 0;
}
