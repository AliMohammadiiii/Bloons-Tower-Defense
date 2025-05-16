#pragma once
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include "../constants.hpp"

class Hud {
public:
    Hud(int& lives, int& score);

    void update();                     
    void draw(sf::RenderWindow&) const;

private:
    int& m_lives;
    int& m_score;

    sf::Text m_txtLives;
    sf::Text m_txtScore;
};
