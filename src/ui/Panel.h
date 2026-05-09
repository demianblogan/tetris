#pragma once

#include <memory>
#include <optional>
#include <SFML/Graphics/Sprite.hpp>
#include "Element.h"
#include "Label.h"

namespace UI
{
	class Panel final : public Element
	{
	private:
		std::unique_ptr<Label> label;
		std::optional<sf::Sprite> backgroundSprite;

		sf::Vector2f preferredSize;

	public:
		explicit Panel(const sf::Sprite& sprite);

		void SetLabel(std::unique_ptr<Label> label);
		void SetPreferredSize(sf::Vector2f size);

		[[nodiscard]] sf::Vector2f Measure() const override;
		void Arrange(sf::Vector2f position, sf::Vector2f size) override;

		void Render(sf::RenderWindow& window) const override;
	};
}