#pragma once

#include <SFML/Graphics/Text.hpp>

#include "Element.h"

namespace UI
{
	class Label final : public Element
	{
	private:
		sf::Text text;

	public:
		Label(const sf::Font& font, const sf::String& string, unsigned int characterSize);

		void SetString(const sf::String& string);
		void SetFillColor(sf::Color color);

		[[nodiscard]] sf::String GetString() const;
		[[nodiscard]] float GetVisualHeight() const;

		[[nodiscard]] sf::Vector2f Measure() const override;
		void Arrange(sf::Vector2f position, sf::Vector2f size) override;

		void Render(sf::RenderTarget& target) const override;
	};
}