#include "Button.h"

Button::Button(const sf::Font& font, const std::string& text, unsigned int characterSize, MainMenuAction action)
	: label(font, text, characterSize), action(action)
{
	UpdateColor();
}

void Button::SetPosition(sf::Vector2f position)
{
	label.SetPosition(position);
}

void Button::CenterHorizontally(float width)
{
	label.CenterHorizontally(width);
}

void Button::SetHovered(bool hovered)
{
	this->hovered = hovered;

	UpdateColor();
}

void Button::SetSelected(bool selected)
{
	this->selected = selected;

	UpdateColor();
}

bool Button::Contains(sf::Vector2f point) const
{
	return label.GetBounds().contains(point);
}

float Button::GetHeight() const
{
	return HEIGHT;
}

MainMenuAction Button::GetAction() const
{
	return action;
}

void Button::Render(sf::RenderWindow& window) const
{
	label.Render(window);
}

void Button::UpdateColor()
{
	if (hovered)
	{
		label.SetFillColor(sf::Color::Yellow);
	}
	else if (selected)
	{
		label.SetFillColor(sf::Color::Cyan);
	}
	else
	{
		label.SetFillColor(sf::Color::White);
	}
}