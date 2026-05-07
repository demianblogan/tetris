#pragma once

#include <array>

#include "../core/Context.h"
#include "../core/State.h"
#include "../statistics/HighScoreManager.h"
#include "../ui/UILabel.h"
#include "../ui/UILayout.h"

class StatisticsState : public State
{
private:
    Context& context;
    HighScoreManager highScoreManager;

    UILabel titleLabel;
    UILayout scoresLayout;

    std::array<UILabel*, HighScoreManager::MAX_RECORDS> scoreLabels;

    UILabel escapeLabel;
    UILabel deleteLabel;

    void UpdateScoreLabels();

public:
    StatisticsState(Context& context);

    void ProcessEvents(sf::RenderWindow& window) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
};