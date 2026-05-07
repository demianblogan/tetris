#include "StatisticsState.h"

#include <optional>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../core/StateMachine.h"
#include "MainMenuState.h"

StatisticsState::StatisticsState(Context& context)
	: context(context)
	, highScoreManager(Data::Paths::Scores)
	, titleLabel(context.fonts.Get(Assets::FontID::Main), "TOP-" + std::to_string(HighScoreManager::MAX_RECORDS) + " best players", 120)
	, scoresLayout(UILayout::Orientation::Vertical)
	, escapeLabel(context.fonts.Get(Assets::FontID::Main), "ESC - return to main menu", 50)
	, deleteLabel(context.fonts.Get(Assets::FontID::Main), "DEL - delete all records", 50)
{
	highScoreManager.Load();

	const float windowWidth = context.window.getView().getSize().x;

	// =========================
	// Title
	// =========================

	titleLabel.SetPosition({ 0.f, 150.f });
	titleLabel.CenterHorizontally(windowWidth);

	// =========================
	// Scores layout
	// =========================

	scoresLayout.SetGap(30.f);

	for (std::size_t i = 0; i < HighScoreManager::MAX_RECORDS; i++)
	{
		auto label = std::make_unique<UILabel>(context.fonts.Get(Assets::FontID::Main), "...", 70);
		scoreLabels[i] = label.get();
		scoresLayout.Add(std::move(label));
	}

	UpdateScoreLabels();

	const float layoutWidth = scoresLayout.GetSize().x;
	scoresLayout.SetPosition({ (windowWidth - layoutWidth) / 2.f, 350.f });

	// =========================
	// Bottom labels
	// =========================

	escapeLabel.SetPosition({ 0.f, 900.f });
	escapeLabel.CenterHorizontally(windowWidth);
	deleteLabel.SetPosition({ 0.f, 970.f });
	deleteLabel.CenterHorizontally(windowWidth);
}

void StatisticsState::UpdateScoreLabels()
{
	const std::vector<HighScoreEntry>& records = highScoreManager.GetRecords();

	const float windowWidth = context.window.getView().getSize().x;

	for (std::size_t i = 0; i < HighScoreManager::MAX_RECORDS; i++)
	{
		if (i < records.size())
		{
			scoreLabels[i]->SetString(std::to_string(i + 1) + ". " + records[i].playerName + " = " + std::to_string(records[i].score));
		}
		else
		{
			scoreLabels[i]->SetString(std::to_string(i + 1) + ". ...");
		}
	}
}

void StatisticsState::ProcessEvents(sf::RenderWindow& window)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			switch (keyPressed->scancode)
			{
			case sf::Keyboard::Scancode::Escape:
				context.stateMachine.ChangeState(std::make_unique<MainMenuState>(context));
				return;

			case sf::Keyboard::Scancode::Delete:
				highScoreManager.Clear();
				highScoreManager.Save();
				UpdateScoreLabels();
				break;
			}
		}
	}
}

void StatisticsState::Update(float deltaTime)
{
	// No code
}

void StatisticsState::Render(sf::RenderWindow& window)
{
	titleLabel.Render(window);
	scoresLayout.Render(window);
	escapeLabel.Render(window);
	deleteLabel.Render(window);
}