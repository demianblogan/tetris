#pragma once

#include <array>
#include "../core/Context.h"
#include "../core/State.h"
#include "../statistics/HighScoreManager.h"
#include "../ui/Layout.h"
#include "../ui/Label.h"
#include "../ui/Spacer.h"
#include <SFML/Graphics/Sprite.hpp>

class StatisticsState final : public State
{
private:
	Context& context;

	HighScoreManager highScoreManager;
	UI::Layout rootLayout;
	std::array<UI::Label*, HighScoreManager::MAX_RECORDS> scoreLabels{};

	sf::Sprite backgroundSprite;

	void UpdateScoreLabels();
	void UpdateLayout();

public:
	explicit StatisticsState(Context& context);

	void ProcessEvents(sf::RenderWindow& window) override;
	void Update(float deltaTime) override;
	void Render(sf::RenderTarget& target) override;
};