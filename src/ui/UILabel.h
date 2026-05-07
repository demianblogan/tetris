#pragma once

#include <string>
#include <SFML/Graphics/Text.hpp>
#include "UIElement.h"

class UILabel : public UIElement
{
private:
	sf::Text text;

	void UpdateOrigin();
public:
	UILabel(const sf::Font& font, const std::string& string, unsigned int characterSize);

	void SetString(const std::string& string);
	void SetCharacterSize(unsigned int characterSize);
	void SetFillColor(const sf::Color& color);
	void SetPosition(sf::Vector2f position) override;

	[[nodiscard]] sf::Vector2f GetPosition() const override;
	[[nodiscard]] sf::Vector2f GetSize() const override;
	[[nodiscard]] const sf::FloatRect GetBounds() const;

	void CenterHorizontally(float width);
	void Render(sf::RenderWindow& window) const override;
};