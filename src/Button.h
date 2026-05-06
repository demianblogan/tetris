#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Label.h"
#include "MenuAction.h"

class Button
{
private:
	static constexpr float HEIGHT = 80.f;

	Label label;
	MainMenuAction action;

	bool hovered = false;
	bool selected = false;

private:
	void UpdateColor();

public:
	Button(const sf::Font& font, const std::string& text, unsigned int characterSize, MainMenuAction action);

	void SetPosition(sf::Vector2f position);
	void CenterHorizontally(float width);
	void SetHovered(bool hovered);
	void SetSelected(bool selected);

	[[nodiscard]] bool Contains(sf::Vector2f point) const;
	[[nodiscard]] float GetHeight() const;
	[[nodiscard]] MainMenuAction GetAction() const;

	void Render(sf::RenderWindow& window) const;
};