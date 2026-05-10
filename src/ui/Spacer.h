#pragma once

#include "Element.h"

namespace UI
{
	class Spacer final : public Element
	{
	private:
		sf::Vector2f preferredSize;

	public:
		explicit Spacer(sf::Vector2f size);

		[[nodiscard]] sf::Vector2f Measure() const override;
		void Render(sf::RenderTarget& target) const override;
	};
}