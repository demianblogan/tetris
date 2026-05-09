#pragma once

#include <SFML/System/String.hpp>

struct HighScoreEntry
{
    sf::String playerName;
    int score = 0;
};