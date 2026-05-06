#include "Label.h"

#include <SFML/Graphics/RenderWindow.hpp>

Label::Label(const sf::Font& font, const std::string& string, unsigned int characterSize)
	: text(font, string, characterSize)
{
	// No code
}

void Label::SetString(const std::string& string)
{
	text.setString(string);
}

void Label::SetPosition(sf::Vector2f position)
{
	text.setPosition(position);
}

void Label::Move(sf::Vector2f offset)
{
	text.move(offset);
}

void Label::CenterHorizontally(float width)
{
	const sf::FloatRect bounds = text.getLocalBounds();

	const sf::Vector2f origin
	{
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	};

	const sf::Vector2f position
	{
		width / 2.f,
		text.getPosition().y
	};

	text.setOrigin(origin);
	text.setPosition(position);
}

void Label::SetFillColor(const sf::Color& color)
{
	text.setFillColor(color);
}

const sf::FloatRect Label::GetBounds() const
{
	return text.getGlobalBounds();
}

sf::Vector2f Label::GetPosition() const
{
	return text.getPosition();
}

void Label::Render(sf::RenderWindow& window) const
{
	window.draw(text);
}