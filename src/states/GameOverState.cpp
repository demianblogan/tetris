#include "GameOverState.h"

#include <optional>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../audio/AudioPlayer.h"
#include "../core/StateMachine.h"
#include "../resources/Assets.h"
#include "../statistics/HighScoreManager.h"
#include "GameState.h"
#include "MainMenuState.h"

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

	auto menuLayoutElement = std::make_unique<UI::Layout>(UI::Layout::Orientation::Vertical);
	menuLayoutElement->SetGap(MenuGap);
	menuLayoutElement->SetHorizontalAlignment(UI::Layout::Alignment::Center);
	menuLayout = menuLayoutElement.get();

	// =====================================================
	// High score UI
	// =====================================================

	if (isHighScore)
	{
		// =================================================
		// High score label
		// =================================================
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

		// =================================================
		// Name input layout
		// =================================================
		{
			auto layout = std::make_unique<UI::Layout>(UI::Layout::Orientation::Horizontal);
			layout->SetGap(20.f);
			layout->SetHorizontalAlignment(UI::Layout::Alignment::Center);
			layout->SetVerticalAlignment(UI::Layout::Alignment::End);

			// =============================================
			// Enter name label
			// =============================================
			{
				auto label = std::make_unique<UI::Label>(
					context.fonts.Get(Assets::FontID::Main),
					"Enter your name:",
					70
				);
				label->SetFillColor(sf::Color::White);
				layout->Add(std::move(label));
			}

			// =============================================
			// Player name label
			// =============================================
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
		rootLayout.Add(std::move(menuLayoutElement));
	}
	else
	{
		CreateMenuButton("Restart", MenuAction::RestartGame);
		CreateMenuButton("Main Menu", MenuAction::MainMenu);

		rootLayout.Add(std::move(menuLayoutElement));
	}

	UpdateSelection();
	UpdateSaveButtonState();
	UpdateLayout();

	context.music.Get(Assets::MusicID::Gameplay).stop();
	sf::Music& music = context.music.Get(Assets::MusicID::GameOver);
	if (music.getStatus() != sf::Music::Status::Playing)
	{
		music.play();
	}
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
			else if (character >= 32 && character != 127)
			{
				if (playerName.getSize() >= MAX_NAME_LENGTH)
				{
					continue;
				}

				if (character == U' ' && playerName.isEmpty())
				{
					continue;
				}

				playerName += character;
			}

			playerNameLabel->SetString(playerName + "_");
			UpdateSaveButtonState();
		}
	}
}

void GameOverState::Update(float deltaTime)
{
	// No code
}

void GameOverState::Render(sf::RenderTarget& target)
{
	// =====================================================
	// Background overlay
	// =====================================================

	sf::RectangleShape overlay;
	overlay.setPosition({ 0.f, 0.f });
	overlay.setSize(target.getView().getSize());
	overlay.setFillColor(sf::Color(0, 0, 0, 220));

	target.draw(overlay);

	// =====================================================
	// UI
	// =====================================================

	sf::Shader& glowShader = context.shaders.Get(Assets::ShaderID::Glow);
	rootLayout.Render(target, &glowShader, context.totalTime);
}

void GameOverState::CreateMenuButton(const sf::String& text, MenuAction action)
{
	sf::Sprite buttonSprite(context.textures.Get(Assets::TextureID::ButtonBackground));

	auto button = std::make_unique<UI::Button>(buttonSprite);
	button->SetPreferredSize({ ButtonWidth,	ButtonHeight });
	button->SetWidthPixels(ButtonWidth);
	button->SetHeightPixels(ButtonHeight);
	button->SetLabel(std::make_unique<UI::Label>(context.fonts.Get(Assets::FontID::Main), text, ButtonTextSize));

	if (action == MenuAction::SaveRecord)
	{
		button->SetNormalStyle(
			{
				.backgroundColor = sf::Color(80, 80, 80),
				.textColor = sf::Color(100, 100, 100)
			}
		);
	}
	else
	{
		button->SetNormalStyle(
			{
				.backgroundColor = sf::Color(140, 140, 140),
				.textColor = sf::Color::White
			}
		);
	}

	button->SetSelectedStyle(
		{
			.backgroundColor = sf::Color(200, 200, 200),
			.textColor = sf::Color::Yellow
		}
	);

	UI::Button* buttonPointer = button.get();
	if (action == MenuAction::SaveRecord)
	{
		saveButton = buttonPointer;
	}

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

		const sf::String trimmedName = TrimPlayerName(playerName);
		context.highScores.AddRecord({ trimmedName, finalScore });
		context.highScores.Save();

		context.stateMachine.ClearStates();
		context.stateMachine.PushState(std::make_unique<MainMenuState>(context));
		break;
	}
}

void GameOverState::UpdateSaveButtonState()
{
	if (saveButton == nullptr)
	{
		return;
	}

	saveButton->SetSelected(IsPlayerNameValid());
}

sf::String GameOverState::TrimPlayerName(const sf::String& string) const
{
	std::size_t start = 0;
	std::size_t end = string.getSize();

	while (start < end && string[start] == U' ')
	{
		start++;
	}

	while (end > start && string[end - 1] == U' ')
	{
		end--;
	}

	return string.substring(start, end - start);
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