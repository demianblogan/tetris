#pragma once

namespace sf
{
    class RenderWindow;
}

class State
{
public:
    virtual ~State() = default;

    virtual void ProcessEvents(sf::RenderWindow& window) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
};