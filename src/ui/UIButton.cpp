#include "UIButton.h"

#include <SFML/Graphics/RenderWindow.hpp>

UIButton::UIButton(const sf::Vector2f& size, const sf::Color& fillColor)
	: rectangleBackground(sf::RectangleShape(size))
{
	rectangleBackground->setFillColor(fillColor);
}

UIButton::UIButton(const sf::Sprite& sprite)
	: spriteBackground(sprite)
{
	// No code
}

UIButton::UIButton(const sf::Vector2f& size, const sf::Color& fillColor, std::unique_ptr<UILabel> label)
	: UIButton(size, fillColor)
{
	this->label = std::move(label);
	UpdateLabelPosition();
}

UIButton::UIButton(const sf::Sprite& sprite, std::unique_ptr<UILabel> label)
	: UIButton(sprite)
{
	this->label = std::move(label);
	UpdateLabelPosition();
}

void UIButton::SetLabel(std::unique_ptr<UILabel> label)
{
	this->label = std::move(label);
	UpdateLabelPosition();
}

UILabel* UIButton::GetLabel()
{
	return label.get();
}

const UILabel* UIButton::GetLabel() const
{
	return label.get();
}

void UIButton::SetNormalStyle(const Style& style)
{
	normalStyle = style;
	UpdateVisualState();
}

void UIButton::SetHoveredStyle(const Style& style)
{
	hoveredStyle = style;
	UpdateVisualState();
}

void UIButton::SetSelectedStyle(const Style& style)
{
	selectedStyle = style;
	UpdateVisualState();
}

void UIButton::SetHovered(bool hovered)
{
	this->hovered = hovered;
	UpdateVisualState();
}

void UIButton::SetSelected(bool selected)
{
	this->selected = selected;
	UpdateVisualState();
}

bool UIButton::IsHovered() const
{
	return hovered;
}

bool UIButton::IsSelected() const
{
	return selected;
}

void UIButton::SetPosition(sf::Vector2f position)
{
	this->position = position;

	if (rectangleBackground.has_value())
	{
		rectangleBackground->setPosition(position);
	}

	if (spriteBackground.has_value())
	{
		spriteBackground->setPosition(position);
	}

	UpdateLabelPosition();
}

sf::Vector2f UIButton::GetPosition() const
{
	return position;
}

sf::Vector2f UIButton::GetSize() const
{
	if (rectangleBackground.has_value())
	{
		return rectangleBackground->getSize();
	}

	if (spriteBackground.has_value())
	{
		return spriteBackground->getGlobalBounds().size;
	}

	if (label != nullptr)
	{
		return label->GetSize();
	}

	return { 0.f, 0.f };
}

bool UIButton::Contains(sf::Vector2f point) const
{
	if (rectangleBackground.has_value())
	{
		return rectangleBackground->getGlobalBounds().contains(point);
	}

	if (spriteBackground.has_value())
	{
		return spriteBackground->getGlobalBounds().contains(point);
	}

	if (label != nullptr)
	{
		return label->GetBounds().contains(point);
	}

	return false;
}

void UIButton::Render(sf::RenderWindow& window) const
{
	if (rectangleBackground.has_value())
	{
		window.draw(*rectangleBackground);
	}

	if (spriteBackground.has_value())
	{
		window.draw(*spriteBackground);
	}

	if (label != nullptr)
	{
		label->Render(window);
	}
}

void UIButton::UpdateVisualState()
{
	ApplyStyle(normalStyle);

	if (selected)
	{
		ApplyStyle(selectedStyle);
	}

	if (hovered)
	{
		ApplyStyle(hoveredStyle);
	}
}

void UIButton::UpdateLabelPosition()
{
	if (label == nullptr)
	{
		return;
	}

	const sf::Vector2f buttonSize = GetSize();
	const sf::Vector2f labelSize = label->GetSize();

	label->SetPosition(
		{
			position.x + (buttonSize.x - labelSize.x) / 2.f,
			position.y + (buttonSize.y - labelSize.y) / 2.f
		}
	);
}

void UIButton::ApplyStyle(const Style& style)
{
	if (rectangleBackground.has_value())
	{
		if (style.backgroundColor.has_value())
		{
			rectangleBackground->setFillColor(*style.backgroundColor);
		}

		if (style.outlineColor.has_value())
		{
			rectangleBackground->setOutlineColor(*style.outlineColor);
		}

		if (style.outlineThickness.has_value())
		{
			rectangleBackground->setOutlineThickness(*style.outlineThickness);
		}
	}

	if (spriteBackground.has_value())
	{
		if (style.backgroundColor.has_value())
		{
			spriteBackground->setColor(*style.backgroundColor);
		}
	}

	if (label != nullptr && style.textColor.has_value())
	{
		label->SetFillColor(*style.textColor);
	}
}