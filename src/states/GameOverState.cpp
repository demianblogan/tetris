
#include "GameOverState.h"

#include <optional>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../audio/AudioPlayer.h"
#include "../core/StateMachine.h"
#include "../resources/Assets.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "../statistics/HighScoreManager.h"

namespace
{
	constexpr float TopSpacing = 220.f;
	constexpr float MenuGap = 30.f;
	constexpr float ButtonWidth = 500.f;
	constexpr float ButtonHeight = 120.f;
	constexpr unsigned int TitleSize = 220;
	constexpr unsigned int ScoreSize = 70;
	constexpr unsigned int ButtonTextSize = 80;
}

GameOverState::GameOverState(Context& context, int finalScore)
	: context(context)
	, rootLayout(UI::Layout::Orientation::Vertical)
	, finalScore(finalScore)
	, isHighScore(context.highScores.IsHighScore(finalScore))
{
	rootLayout.SetHorizontalAlignment(UI::Layout::Alignment::Center);
	rootLayout.SetVerticalAlignment(UI::Layout::Alignment::Start);
	rootLayout.SetGap(60.f);

	// =====================================================
	// Top spacer
	// =====================================================

	rootLayout.Add(std::make_unique<UI::Spacer>(sf::Vector2f{ 0.f, TopSpacing }));

	// =====================================================
	// Title
	// =====================================================
	{
		auto title = std::make_unique<UI::Label>(context.fonts.Get(Assets::FontID::Main), "Game Over", TitleSize);
		title->SetFillColor(sf::Color::White);
		rootLayout.Add(std::move(title));
	}

	// =====================================================
	// Score label
	// =====================================================
	{
		auto label = std::make_unique<UI::Label>(
			context.fonts.Get(Assets::FontID::Main),
			"Your score: " + std::to_string(finalScore),
			ScoreSize
		);
		label->SetFillColor(sf::Color::White);
		scoreLabel = label.get();
		rootLayout.Add(std::move(label));
	}

	// =====================================================
	// Menu layout
	// =====================================================
	{
		auto layout = std::make_unique<UI::Layout>(UI::Layout::Orientation::Vertical);
		layout->SetGap(MenuGap);
		layout->SetHorizontalAlignment(UI::Layout::Alignment::Center);
		menuLayout = layout.get();
		rootLayout.Add(std::move(layout));
	}

	if (isHighScore)
	{
		// =====================================================
		// High score label
		// =====================================================
		{
			auto label = std::make_unique<UI::Label>(
				context.fonts.Get(Assets::FontID::Main),
				"You broke the record!",
				70
			);
			label->SetFillColor(sf::Color(255, 215, 0));
			highScoreLabel = label.get();
			rootLayout.Add(std::move(label));
		}

		// =====================================================
		// Name input layout
		// =====================================================
		{
			auto layout = std::make_unique<UI::Layout>(UI::Layout::Orientation::Horizontal);
			layout->SetGap(20.f);
			layout->SetHorizontalAlignment(UI::Layout::Alignment::Center);
			layout->SetVerticalAlignment(UI::Layout::Alignment::End);

			// =================================================
			// Enter name label
			// =================================================
			{
				auto label = std::make_unique<UI::Label>(
					context.fonts.Get(Assets::FontID::Main),
					"Enter your name:",
					70
				);
				label->SetFillColor(sf::Color::White);
				layout->Add(std::move(label));
			}

			// =================================================
			// Player name label
			// =================================================
			{
				auto label = std::make_unique<UI::Label>(
					context.fonts.Get(Assets::FontID::Main),
					"_",
					70
				);

				label->SetFillColor(sf::Color::White);
				playerNameLabel = label.get();
				layout->Add(std::move(label));
			}

			rootLayout.Add(std::move(layout));
		}

		CreateMenuButton("Save", MenuAction::SaveRecord);
	}
	else
	{
		CreateMenuButton("Restart", MenuAction::RestartGame);
		CreateMenuButton("Main Menu", MenuAction::MainMenu);
	}

	UpdateSelection();
	UpdateLayout();
}

void GameOverState::ProcessEvents(sf::RenderWindow& window)
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
			case sf::Keyboard::Scancode::Up:
				if (!isHighScore)
				{
					SelectPreviousMenuItem();
					context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
				}
				break;

			case sf::Keyboard::Scancode::Down:
				if (!isHighScore)
				{
					SelectNextMenuItem();
					context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
				}
				break;

			case sf::Keyboard::Scancode::Enter:
				context.audioPlayer.Play(Assets::SoundID::MenuItemPressed);
				ActivateSelectedButton();
				return;
			}
		}
		else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
		{
			if (!isHighScore)
			{
				continue;
			}

			char32_t character = textEntered->unicode;

			if (character == U'\b') // Backspace
			{
				if (!playerName.isEmpty())
				{
					playerName.erase(playerName.getSize() - 1, 1);
				}
			}
			else if (character >= 32 && character != 127) // Printable characters
			{
				if (playerName.getSize() < MAX_NAME_LENGTH)
				{
					playerName += character;
				}
			}

			playerNameLabel->SetString(playerName + "_");
		}
	}
}

void GameOverState::Update(float deltaTime)
{
	// No code
}

void GameOverState::Render(sf::RenderWindow& window)
{
	// =====================================================
	// Background overlay
	// =====================================================

	sf::RectangleShape overlay;
	overlay.setPosition({ 0.f, 0.f });
	overlay.setSize(window.getView().getSize());
	overlay.setFillColor(sf::Color(0, 0, 0, 220));

	window.draw(overlay);

	// =====================================================
	// UI
	// =====================================================

	rootLayout.Render(window);
}

void GameOverState::CreateMenuButton(const sf::String& text, MenuAction action)
{
	auto button = std::make_unique<UI::Button>(
		sf::Vector2f
		{
			ButtonWidth,
			ButtonHeight
		}
	);

	button->SetLabel(std::make_unique<UI::Label>(
		context.fonts.Get(Assets::FontID::Main),
		text,
		ButtonTextSize)
	);

	button->SetNormalStyle(
		{
			.backgroundColor = sf::Color(60, 60, 60),
			.textColor = sf::Color::White
		}
	);

	button->SetSelectedStyle(
		{
			.backgroundColor = sf::Color::White,
			.textColor = sf::Color::Black
		}
	);

	UI::Button* buttonPointer = button.get();

	menuLayout->Add(std::move(button));

	buttons.push_back(
		{
			.button = buttonPointer,
			.action = action
		}
	);
}

void GameOverState::SelectPreviousMenuItem()
{
	selectedIndex--;

	if (selectedIndex < 0)
	{
		selectedIndex = static_cast<int>(buttons.size()) - 1;
	}

	UpdateSelection();
}

void GameOverState::SelectNextMenuItem()
{
	selectedIndex++;

	if (selectedIndex >= static_cast<int>(buttons.size()))
	{
		selectedIndex = 0;
	}

	UpdateSelection();
}

void GameOverState::UpdateSelection()
{
	for (std::size_t i = 0; i < buttons.size(); i++)
	{
		buttons[i].button->SetSelected(i == static_cast<std::size_t>(selectedIndex));
	}
}

void GameOverState::ActivateSelectedButton()
{
	switch (buttons[selectedIndex].action)
	{
	case MenuAction::RestartGame:
		context.stateMachine.ClearStates();
		context.stateMachine.PushState(std::make_unique<GameState>(context));
		break;

	case MenuAction::MainMenu:
		context.stateMachine.ClearStates();
		context.stateMachine.PushState(std::make_unique<MainMenuState>(context));
		break;

	case MenuAction::SaveRecord:
		if (!IsPlayerNameValid())
		{
			break;
		}

		context.highScores.AddRecord({ playerName, finalScore });
		context.highScores.Save();

		context.stateMachine.ClearStates();
		context.stateMachine.PushState(std::make_unique<MainMenuState>(context));
		break;
	}
}

bool GameOverState::IsPlayerNameValid() const
{
	for (char32_t character : playerName)
	{
		if (character != U' ')
		{
			return true;
		}
	}

	return false;
}

void GameOverState::UpdateLayout()
{
	const sf::Vector2f viewSize = context.window.getView().getSize();
	rootLayout.Arrange({ 0.f, 0.f }, viewSize);
}