#include "UILabel.h"

#include <SFML/Graphics/RenderWindow.hpp>

void UILabel::UpdateOrigin()
{
	const sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.position);
}

UILabel::UILabel(const sf::Font& font, const std::string& string, unsigned int characterSize)
	: text(font, string, characterSize)
{
	UpdateOrigin();
}

void UILabel::SetString(const std::string& string)
{
	text.setString(string);
	UpdateOrigin();
}

void UILabel::SetCharacterSize(unsigned int characterSize)
{
	text.setCharacterSize(characterSize);
	UpdateOrigin();
}

void UILabel::SetFillColor(const sf::Color& color)
{
	text.setFillColor(color);
}

void UILabel::SetPosition(sf::Vector2f position)
{
	text.setPosition(position);
}

sf::Vector2f UILabel::GetPosition() const
{
	return text.getPosition();
}

sf::Vector2f UILabel::GetSize() const
{
	return text.getLocalBounds().size;
}

const sf::FloatRect UILabel::GetBounds() const
{
	return text.getGlobalBounds();
}

void UILabel::CenterHorizontally(float width)
{
	text.setPosition({ (width - GetSize().x) / 2.f, text.getPosition().y });
}

void UILabel::Render(sf::RenderWindow& window) const
{
	window.draw(text);
}