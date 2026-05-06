#pragma once

#include <SFML/Graphics/Text.hpp>

namespace sf
{
	class RenderWindow;
}

class Label
{
private:
	sf::Text text;

public:
	Label(const sf::Font& font, const std::string& string, unsigned int characterSize);

	void SetString(const std::string& string);
	void SetPosition(sf::Vector2f position);
	void Move(sf::Vector2f offset);
	void CenterHorizontally(float width);
	void SetFillColor(const sf::Color& color);

	[[nodiscard]] const sf::FloatRect GetBounds() const;
	[[nodiscard]] sf::Vector2f GetPosition() const;

	void Render(sf::RenderWindow& window) const;
};