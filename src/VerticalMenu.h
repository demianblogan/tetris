#pragma once

#include <vector>

#include "Button.h"

class VerticalMenu
{
private:
	std::vector<Button> buttons;
	std::size_t selectedIndex = 0;
	float spacing = 30.f;

public:
	void AddButton(Button button);
	void SetPosition(sf::Vector2f position);
	void CenterHorizontally(float width);
	void MoveSelectionUp();
	void MoveSelectionDown();
	bool UpdateMouseHover(sf::Vector2f mousePosition);

	[[nodiscard]] MainMenuAction GetSelectedAction() const;

	void Render(sf::RenderWindow& window) const;
};