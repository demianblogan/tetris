#include "StatisticsState.h"

#include <optional>

#include <SFML/Graphics/RenderWindow.hpp>

#include "MainMenuState.h"
#include "StateMachine.h"

StatisticsState::StatisticsState(Context& context)
	: context(context)
	, highScoreManager(Data::Paths::Scores)
	, titleLabel(context.fonts.Get(Assets::FontID::Main), "TOP-" + std::to_string(HighScoreManager::MAX_RECORDS) + " best players", 120)
	, escapeLabel(context.fonts.Get(Assets::FontID::Main), "ESC - return to main menu", 50)
	, deleteLabel(context.fonts.Get(Assets::FontID::Main), "DEL - delete all records", 50)
	, scoreLabels
	{
		Label(context.fonts.Get(Assets::FontID::Main), "...", 70),
		Label(context.fonts.Get(Assets::FontID::Main), "...", 70),
		Label(context.fonts.Get(Assets::FontID::Main), "...", 70),
		Label(context.fonts.Get(Assets::FontID::Main), "...", 70),
		Label(context.fonts.Get(Assets::FontID::Main), "...", 70)
	}
{
	highScoreManager.Load();

	const float windowWidth = context.window.getView().getSize().x;

	titleLabel.SetPosition({ 0.f, 150.f });
	titleLabel.CenterHorizontally(windowWidth);

	for (std::size_t i = 0; i < HighScoreManager::MAX_RECORDS; i++)
	{
		scoreLabels[i].SetPosition({ 0.f, 350.f + i * 100.f });
		scoreLabels[i].CenterHorizontally(windowWidth);
	}

	UpdateScoreLabels();

	escapeLabel.SetPosition({ 0.f, 900.f });
	escapeLabel.CenterHorizontally(windowWidth);

	deleteLabel.SetPosition({ 0.f, 970.f });
	deleteLabel.CenterHorizontally(windowWidth);
}

void StatisticsState::UpdateScoreLabels()
{
	const std::vector<HighScoreEntry>& records = highScoreManager.GetRecords();

	for (std::size_t i = 0; i < HighScoreManager::MAX_RECORDS; i++)
	{
		if (i < records.size())
		{
			scoreLabels[i].SetString(std::to_string(i + 1) + ". " + records[i].playerName + " = " + std::to_string(records[i].score));
		}
		else
		{
			scoreLabels[i].SetString(std::to_string(i + 1) + ". ...");
		}

		scoreLabels[i].CenterHorizontally(context.window.getView().getSize().x);
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
	// No code needed for updating the statistics state in this implementation.
}

void StatisticsState::Render(sf::RenderWindow& window)
{
	titleLabel.Render(window);

	for (const Label& label : scoreLabels)
	{
		label.Render(window);
	}

	escapeLabel.Render(window);
	deleteLabel.Render(window);
}