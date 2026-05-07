#include "MainMenuState.h"

#include <optional>

#include <SFML/Graphics/RenderWindow.hpp>

#include "../audio/AudioPlayer.h"
#include "../core/StateMachine.h"

#include "SettingsState.h"
#include "StatisticsState.h"

MainMenuState::MainMenuState(Context& context)
	: context(context)
	, titleLabel(context.fonts.Get(Assets::FontID::Main), "Tetris", 300)
	, menuLayout(UILayout::Orientation::Vertical)
{
	// =========================
	// Title
	// =========================

	titleLabel.SetPosition({ 0.f, 100.f });
	titleLabel.CenterHorizontally(context.window.getView().getSize().x);

	// =========================
	// Layout
	// =========================

	menuLayout.SetGap(30.f);
	menuLayout.SetPosition({ 700.f, 400.f });

	// =========================
	// Start Game button
	// =========================

	auto startGameButton = std::make_unique<UIButton>(sf::Vector2f{ 500.f, 120.f }, sf::Color(60, 60, 60));
	startGameButton->SetLabel(std::make_unique<UILabel>(context.fonts.Get(Assets::FontID::Main), "Start Game", 80));
	startGameButton->SetNormalStyle({ .backgroundColor = sf::Color(60, 60, 60), .textColor = sf::Color::White });
	startGameButton->SetSelectedStyle({ .backgroundColor = sf::Color::White, .textColor = sf::Color::Black });

	UIButton* startGameButtonPointer = startGameButton.get();
	menuLayout.Add(std::move(startGameButton));
	buttons.push_back({ startGameButtonPointer, MenuAction::StartGame });

	// =========================
	// Options button
	// =========================

	auto optionsButton = std::make_unique<UIButton>(sf::Vector2f{ 500.f, 120.f }, sf::Color(60, 60, 60));
	optionsButton->SetLabel(std::make_unique<UILabel>(context.fonts.Get(Assets::FontID::Main), "Options", 80));
	optionsButton->SetNormalStyle({ .backgroundColor = sf::Color(60, 60, 60), .textColor = sf::Color::White });
	optionsButton->SetSelectedStyle({ .backgroundColor = sf::Color::White, .textColor = sf::Color::Black });

	UIButton* optionsButtonPointer = optionsButton.get();
	menuLayout.Add(std::move(optionsButton));
	buttons.push_back({ optionsButtonPointer, MenuAction::Options });

	// =========================
	// Statistics button
	// =========================

	auto statisticsButton = std::make_unique<UIButton>(sf::Vector2f{ 500.f, 120.f }, sf::Color(60, 60, 60));
	statisticsButton->SetLabel(std::make_unique<UILabel>(context.fonts.Get(Assets::FontID::Main), "Statistics", 80));
	statisticsButton->SetNormalStyle({ .backgroundColor = sf::Color(60, 60, 60), .textColor = sf::Color::White });
	statisticsButton->SetSelectedStyle({ .backgroundColor = sf::Color::White, .textColor = sf::Color::Black });

	UIButton* statisticsButtonPointer = statisticsButton.get();
	menuLayout.Add(std::move(statisticsButton));
	buttons.push_back({ statisticsButtonPointer, MenuAction::Statistics });

	// =========================
	// Exit button
	// =========================

	auto exitButton = std::make_unique<UIButton>(sf::Vector2f{ 500.f, 120.f }, sf::Color(60, 60, 60));
	exitButton->SetLabel(std::make_unique<UILabel>(context.fonts.Get(Assets::FontID::Main),"Exit",80));
	exitButton->SetNormalStyle({ .backgroundColor = sf::Color(60, 60, 60), .textColor = sf::Color::White });
	exitButton->SetSelectedStyle({ .backgroundColor = sf::Color::White, .textColor = sf::Color::Black });

	UIButton* exitButtonPointer = exitButton.get();
	menuLayout.Add(std::move(exitButton));
	buttons.push_back({ exitButtonPointer, MenuAction::Exit });

	// =========================
	// Initial selection
	// =========================

	UpdateSelection();

	// =========================
	// Music
	// =========================

	sf::Music& music = context.music.Get(Assets::MusicID::MainMenu);
	music.setLooping(true);
	if (music.getStatus() != sf::Music::Status::Playing)
	{
		music.play();
	}
}

void MainMenuState::ProcessEvents(sf::RenderWindow& window)
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
				window.close();
				break;

			case sf::Keyboard::Scancode::Up:
				SelectPreviousMenuItem();
				context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
				break;

			case sf::Keyboard::Scancode::Down:
				SelectNextMenuItem();
				context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
				break;

			case sf::Keyboard::Scancode::Enter:
			case sf::Keyboard::Scancode::Space:
				context.audioPlayer.Play(Assets::SoundID::MenuItemPressed);
				ActivateSelectedButton();
				break;
			}
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				const sf::Vector2f mouseWorldPosition = context.window.mapPixelToCoords(
						{
							mouseButtonPressed->position.x,
							mouseButtonPressed->position.y
						}
					);

				for (std::size_t i = 0; i < buttons.size(); i++)
				{
					if (buttons[i].button->Contains(mouseWorldPosition))
					{
						selectedIndex =	static_cast<int>(i);
						UpdateSelection();
						context.audioPlayer.Play(Assets::SoundID::MenuItemPressed);
						ActivateSelectedButton();
						break;
					}
				}
			}
		}
	}
}

void MainMenuState::Update(float deltaTime)
{
	const sf::Vector2i mousePixelPosition =	sf::Mouse::getPosition(context.window);
	const sf::Vector2f mouseWorldPosition =	context.window.mapPixelToCoords(mousePixelPosition);

	for (std::size_t i = 0; i < buttons.size(); i++)
	{
		const bool hovered = buttons[i].button->Contains(mouseWorldPosition);
		buttons[i].button->SetHovered(hovered);

		if (hovered && selectedIndex != i)
		{
			selectedIndex = static_cast<int>(i);
			UpdateSelection();
			context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
		}
	}
}

void MainMenuState::Render(sf::RenderWindow& window)
{
	titleLabel.Render(window);
	menuLayout.Render(window);
}

void MainMenuState::SelectPreviousMenuItem()
{
	selectedIndex--;

	if (selectedIndex < 0)
	{
		selectedIndex = static_cast<int>(buttons.size()) - 1;
	}

	UpdateSelection();
}

void MainMenuState::SelectNextMenuItem()
{
	selectedIndex++;

	if (selectedIndex >= static_cast<int>(buttons.size()))
	{
		selectedIndex = 0;
	}

	UpdateSelection();
}

void MainMenuState::UpdateSelection()
{
	for (std::size_t i = 0; i < buttons.size(); i++)
	{
		buttons[i].button->SetSelected(i == selectedIndex);
	}
}

void MainMenuState::ActivateSelectedButton()
{
	switch (buttons[selectedIndex].action)
	{
	case MenuAction::StartGame:
		// TODO: Start game
		break;

	case MenuAction::Options:
		// TODO: Implement SettingsState and change state to it
		//context.stateMachine.ChangeState(std::make_unique<SettingsState>(context));
		break;

	case MenuAction::Statistics:
		context.stateMachine.ChangeState(std::make_unique<StatisticsState>(context));
		break;

	case MenuAction::Exit:
		context.window.close();
		break;
	}
}