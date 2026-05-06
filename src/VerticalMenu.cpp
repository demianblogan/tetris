#include "VerticalMenu.h"

void VerticalMenu::AddButton(Button button)
{
	buttons.push_back(std::move(button));

	if (buttons.size() == 1)
	{
		buttons[0].SetSelected(true);
	}
}

void VerticalMenu::SetPosition(sf::Vector2f position)
{
	float currentY = position.y;

	for (Button& button : buttons)
	{
		button.SetPosition({ position.x, currentY });
		currentY += button.GetHeight() + spacing;
	}
}

void VerticalMenu::CenterHorizontally(float width)
{
	for (Button& button : buttons)
	{
		button.CenterHorizontally(width);
	}
}

void VerticalMenu::MoveSelectionUp()
{
	if (buttons.empty())
	{
		return;
	}

	buttons[selectedIndex].SetSelected(false);

	if (selectedIndex == 0)
	{
		selectedIndex = buttons.size() - 1;
	}
	else
	{
		--selectedIndex;
	}

	buttons[selectedIndex].SetSelected(true);
}

void VerticalMenu::MoveSelectionDown()
{
	if (buttons.empty())
	{
		return;
	}

	buttons[selectedIndex].SetSelected(false);

	selectedIndex = (selectedIndex + 1) % buttons.size();

	buttons[selectedIndex].SetSelected(true);
}

bool VerticalMenu::UpdateMouseHover(sf::Vector2f mousePosition)
{
	for (std::size_t i = 0;	i < buttons.size(); i++)
	{
		const bool hovered = buttons[i].Contains(mousePosition);

		buttons[i].SetHovered(hovered);

		if (hovered && selectedIndex != i)
		{
			buttons[selectedIndex].SetSelected(false);

			selectedIndex = i;

			buttons[selectedIndex].SetSelected(true);

			return true;
		}
	}

	return false;
}

MainMenuAction VerticalMenu::GetSelectedAction() const
{
	return buttons[selectedIndex].GetAction();
}

void VerticalMenu::Render(sf::RenderWindow& window) const
{
	for (const Button& button : buttons)
	{
		button.Render(window);
	}
}