// Spacer.cpp

#include "Spacer.h"

namespace UI
{
	Spacer::Spacer(sf::Vector2f size)
		: preferredSize(size)
	{
		SetWidthPixels(size.x);
		SetHeightPixels(size.y);
	}

	sf::Vector2f Spacer::Measure() const
	{
		return preferredSize;
	}

	void Spacer::Arrange(sf::Vector2f position, sf::Vector2f size)
	{
		Element::Arrange(position, size);
	}

	void Spacer::Render(sf::RenderWindow&) const
	{
		// No code
	}
}