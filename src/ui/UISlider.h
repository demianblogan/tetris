#pragma once

#include <variant>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "UIElement.h"

class UISlider : public UIElement
{
public:
	struct RectangleVisual
	{
		sf::Color fillColor = sf::Color::White;
		sf::Color outlineColor = sf::Color::Transparent;
		float outlineThickness = 0.f;
	};

	struct SpriteVisual
	{
		sf::Sprite sprite;
		bool tiled = false;
	};

	using Visual = std::variant<RectangleVisual, SpriteVisual>;

private:
	sf::Vector2f position{ 0.f, 0.f };

	sf::Vector2f trackSize;
	sf::Vector2f handleSize;

	float minimum;
	float maximum;
	float value;

	float step;

	Visual trackVisual;
	Visual fillVisual;
	Visual handleVisual;

	[[nodiscard]] sf::Vector2f GetHandlePosition() const;
	void DrawVisual(sf::RenderWindow& window, const Visual& visual, sf::Vector2f position, sf::Vector2f size) const;

public:
	UISlider(
		sf::Vector2f trackSize,
		sf::Vector2f handleSize,
		float minimum,
		float maximum,
		float value,
		float step,
		Visual trackVisual,
		Visual fillVisual,
		Visual handleVisual
	);

	void SetValue(float value);
	void Increase();
	void Decrease();
	void SetRange(float minimum, float maximum);
	void SetStep(float step);

	[[nodiscard]] float GetValue() const;
	[[nodiscard]] float GetMinimum() const;
	[[nodiscard]] float GetMaximum() const;
	[[nodiscard]] float GetStep() const;

	void SetPosition(sf::Vector2f position) override;
	[[nodiscard]] sf::Vector2f GetPosition() const override;
	[[nodiscard]] sf::Vector2f GetSize() const override;

	[[nodiscard]] float GetNormalizedValue() const;

	void Render(sf::RenderWindow& window) const override;
};