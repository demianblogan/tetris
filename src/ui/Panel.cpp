#include "Panel.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace UI
{
	Panel::Panel(const sf::Sprite& sprite)
		: backgroundSprite(sprite)
	{
		// No code
	}

	void Panel::SetLabel(std::unique_ptr<Label> label)
	{
		this->label = std::move(label);
	}

	void Panel::SetPreferredSize(sf::Vector2f size)
	{
		preferredSize = size;
	}

	sf::Vector2f Panel::Measure() const
	{
		return preferredSize;
	}

	void Panel::Arrange(sf::Vector2f position, sf::Vector2f size)
	{
		Element::Arrange(position, size);

		if (label)
		{
			const sf::Vector2f labelSize = label->Measure();

			label->Arrange(
				{
					position.x + (size.x - labelSize.x) / 2.f,
					position.y + (size.y - labelSize.y) / 2.f
				},
				labelSize
			);
		}
	}

	void Panel::Render(sf::RenderWindow& window) const
	{
		if (backgroundSprite)
		{
			sf::Sprite sprite = *backgroundSprite;

			const sf::FloatRect bounds = sprite.getLocalBounds();

			sprite.setPosition(position);
			sprite.setScale(
				{
					size.x / bounds.size.x,
					size.y / bounds.size.y
				}
			);

			window.draw(sprite);
		}

		if (label)
		{
			label->Render(window);
		}
	}
}