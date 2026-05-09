#pragma once

#include "Element.h"

namespace UI
{
	class Spacer final : public Element
	{
	private:
		sf::Vector2f preferredSize;

	public:
		explicit Spacer(sf::Vector2f size = { 0.f, 0.f });

		[[nodiscard]] sf::Vector2f Measure() const override;
		void Arrange(sf::Vector2f position, sf::Vector2f size) override;
		void Render(sf::RenderTarget& target) const override;
	};
}