#pragma once

#include <variant>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Element.h"

namespace UI
{
	class Slider final : public Element
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
		sf::Vector2f preferredTrackSize;
		sf::Vector2f handleSize;

		float minimum = 0.f;
		float maximum = 1.f;
		float value = 0.f;
		float step = 0.f;

		Visual trackVisual;
		Visual fillVisual;
		Visual handleVisual;

		[[nodiscard]] float GetNormalizedValue() const;
		[[nodiscard]] sf::Vector2f GetTrackPosition() const;
		[[nodiscard]] sf::Vector2f GetTrackSize() const;
		[[nodiscard]] sf::Vector2f GetFillSize() const;
		[[nodiscard]] sf::Vector2f GetHandlePosition() const;
		[[nodiscard]] float GetVisualHeight() const;

		void DrawVisual(sf::RenderWindow& window, const Visual& visual, sf::Vector2f position, sf::Vector2f size) const;

	public:
		Slider(
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
		[[nodiscard]] float GetValue() const;

		void Increase();
		void Decrease();

		void SetRange(float minimum, float maximum);
		void SetStep(float step);

		[[nodiscard]] sf::Vector2f Measure() const override;
		void Arrange(sf::Vector2f position, sf::Vector2f size) override;

		[[nodiscard]] bool Contains(sf::Vector2f point) const;

		void Render(sf::RenderWindow& window) const override;
	};
}