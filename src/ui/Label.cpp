#include "Label.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace UI
{
	Label::Label(const sf::Font& font, const sf::String& string, unsigned int characterSize)
		: text(font, string, characterSize)
	{
		// No code
	}

	void Label::SetString(const sf::String& string)
	{
		text.setString(string);
	}

	void Label::SetFillColor(sf::Color color)
	{
		text.setFillColor(color);
	}

	sf::String Label::GetString() const
	{
		return text.getString();
	}

	float Label::GetVisualHeight() const
	{
		return static_cast<float>(text.getCharacterSize());
	}

	sf::Vector2f Label::Measure() const
	{
		const sf::FloatRect bounds = text.getLocalBounds();

		return
		{
			bounds.size.x,
			bounds.size.y
		};
	}

	void Label::Arrange(sf::Vector2f position, sf::Vector2f size)
	{
		Element::Arrange(position, size);

		const sf::FloatRect bounds = text.getLocalBounds();
		text.setPosition({ position.x, position.y - bounds.position.y });
	}

	void Label::Render(sf::RenderTarget& target) const
	{
		target.draw(text);
	}
}