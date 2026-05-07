#pragma once

#include <memory>
#include <optional>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include "UIElement.h"
#include "UILabel.h"

class UIButton : public UIElement
{
public:
	struct Style
	{
		std::optional<sf::Color> backgroundColor;
		std::optional<sf::Color> textColor;
		std::optional<sf::Color> outlineColor;
		std::optional<float> outlineThickness;
	};

private:
	std::optional<sf::RectangleShape> rectangleBackground;
	std::optional<sf::Sprite> spriteBackground;

	std::unique_ptr<UILabel> label;

	sf::Vector2f position{ 0.f, 0.f };

	bool hovered = false;
	bool selected = false;

	Style normalStyle;
	Style hoveredStyle;
	Style selectedStyle;

	void UpdateVisualState();
	void UpdateLabelPosition();
	void ApplyStyle(const Style& style);

public:
	UIButton() = default;

	UIButton(const sf::Vector2f& size, const sf::Color& fillColor);
	UIButton(const sf::Sprite& sprite);
	UIButton(const sf::Vector2f& size, const sf::Color& fillColor, std::unique_ptr<UILabel> label);
	UIButton(const sf::Sprite& sprite, std::unique_ptr<UILabel> label);

	void SetLabel(std::unique_ptr<UILabel> label);
	[[nodiscard]] UILabel* GetLabel();
	[[nodiscard]] const UILabel* GetLabel() const;

	void SetNormalStyle(const Style& style);
	void SetHoveredStyle(const Style& style);
	void SetSelectedStyle(const Style& style);

	void SetHovered(bool hovered);
	void SetSelected(bool selected);

	[[nodiscard]] bool IsHovered() const;
	[[nodiscard]] bool IsSelected() const;

	void SetPosition(sf::Vector2f position) override;
	[[nodiscard]] sf::Vector2f GetPosition() const override;
	[[nodiscard]] sf::Vector2f GetSize() const override;

	[[nodiscard]] bool Contains(sf::Vector2f point) const;

	void Render(sf::RenderWindow& window) const override;
};