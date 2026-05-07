#include "UILayout.h"

#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>

UILayout::UILayout(Orientation orientation)
	: orientation(orientation)
{
	// No code
}

void UILayout::Add(std::unique_ptr<UIElement> element)
{
	children.push_back(std::move(element));
	UpdateLayout();
}

void UILayout::SetGap(float gap)
{
	this->gap = gap;
	UpdateLayout();
}

void UILayout::SetAlignment(Alignment alignment)
{
	this->alignment = alignment;
	UpdateLayout();
}

void UILayout::SetPosition(sf::Vector2f position)
{
	this->position = position;
	UpdateLayout();
}

sf::Vector2f UILayout::GetPosition() const
{
	return position;
}

sf::Vector2f UILayout::GetSize() const
{
	if (children.empty())
	{
		return { 0.f, 0.f };
	}

	float width = 0.f;
	float height = 0.f;

	if (orientation == Orientation::Vertical)
	{
		for (const auto& child : children)
		{
			const sf::Vector2f childSize = child->GetSize();
			width = std::max(width, childSize.x);
			height += childSize.y;
		}

		height += gap * static_cast<float>(children.size() - 1);
	}
	else
	{
		for (const auto& child : children)
		{
			const sf::Vector2f childSize = child->GetSize();
			width += childSize.x;
			height = std::max(height, childSize.y);
		}

		width += gap * static_cast<float>(children.size() - 1);
	}

	return { width, height };
}

void UILayout::Render(sf::RenderWindow& window) const
{
	for (const auto& child : children)
	{
		child->Render(window);
	}
}

void UILayout::UpdateLayout()
{
	sf::Vector2f currentPosition = position;

	const sf::Vector2f layoutSize = GetSize();

	for (const auto& child : children)
	{
		const sf::Vector2f childSize = child->GetSize();
		sf::Vector2f childPosition = currentPosition;

		if (orientation == Orientation::Vertical)
		{
			switch (alignment)
			{
			case Alignment::Start:
				// Left aligned automatically, so no need to adjust the x position
				break;

			case Alignment::Center:
				childPosition.x += (layoutSize.x - childSize.x) / 2.f;
				break;

			case Alignment::End:
				childPosition.x += layoutSize.x - childSize.x;
				break;
			}

			currentPosition.y += childSize.y + gap;
		}
		else
		{
			switch (alignment)
			{
			case Alignment::Start:
				// Top aligned automatically, so no need to adjust the y position
				break;

			case Alignment::Center:
				childPosition.y += (layoutSize.y - childSize.y) / 2.f;
				break;

			case Alignment::End:
				childPosition.y += layoutSize.y - childSize.y;
				break;
			}

			currentPosition.x += childSize.x + gap;
		}

		child->SetPosition(childPosition);
	}
}