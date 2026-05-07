#pragma once

#include <array>

#include "Context.h"
#include "HighScoreManager.h"
#include "Label.h"
#include "State.h"

class StatisticsState : public State
{
private:
	Context& context;

	HighScoreManager highScoreManager;

	Label titleLabel;
	std::array<Label, HighScoreManager::MAX_RECORDS> scoreLabels;
	Label escapeLabel;
	Label deleteLabel;

	void UpdateScoreLabels();

public:
	StatisticsState(Context& context);

	void ProcessEvents(sf::RenderWindow& window) override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;
};