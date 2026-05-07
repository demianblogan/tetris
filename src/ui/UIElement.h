#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderWindow;
}

class UIElement
{
public:
    virtual ~UIElement() = default;

    virtual void SetPosition(sf::Vector2f position) = 0;
    [[nodiscard]] virtual sf::Vector2f GetPosition() const = 0;
    [[nodiscard]] virtual sf::Vector2f GetSize() const = 0;
    virtual void Render(sf::RenderWindow& window) const = 0;
};