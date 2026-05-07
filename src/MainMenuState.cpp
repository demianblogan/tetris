#include "MainMenuState.h"

#include <optional>

#include <SFML/Graphics/RenderWindow.hpp>

#include "StateMachine.h"
#include "StatisticsState.h"

namespace
{
	void CenterTextHorizontally(sf::Text& text, float windowWidth)
	{
		const sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
		text.setPosition({ windowWidth / 2.f, text.getPosition().y });
	}
}

MainMenuState::MainMenuState(Context& context)
	: context(context)
	, titleLabel(context.fonts.Get(Assets::FontID::Main), "Tetris", 300)
{
	// =========================
	// Title
	// =========================

	titleLabel.SetPosition({ 0.f, 200.f });
	titleLabel.CenterHorizontally(context.window.getView().getSize().x);

	// =========================
	// Menu buttons
	// =========================

	menu.AddButton(Button(context.fonts.Get(Assets::FontID::Main), "Start Game", 100, MainMenuAction::StartGame));
	menu.AddButton(Button(context.fonts.Get(Assets::FontID::Main), "Options", 100, MainMenuAction::Options));
	menu.AddButton(Button(context.fonts.Get(Assets::FontID::Main), "Statistics", 100, MainMenuAction::Statistics));
	menu.AddButton(Button(context.fonts.Get(Assets::FontID::Main), "Exit", 100, MainMenuAction::Exit));

	menu.SetPosition({ 0.f, 500.f });
	menu.CenterHorizontally(context.window.getView().getSize().x);

	// =========================
	// Music
	// =========================

	sf::Music& music = context.music.Get(Assets::MusicID::MainMenu);

	music.setLooping(true);
	music.setVolume(50.f);
	music.play();
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
				menu.MoveSelectionUp();
				context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
				break;

			case sf::Keyboard::Scancode::Down:
				menu.MoveSelectionDown();
				context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
				break;

			case sf::Keyboard::Scancode::Enter:
			case sf::Keyboard::Scancode::Space:
				context.audioPlayer.Play(Assets::SoundID::MenuItemPressed);

				switch (menu.GetSelectedAction())
				{
				case MainMenuAction::StartGame:
					// TODO: Start Game
					break;

				case MainMenuAction::Options:
					// TODO: Options			
					break;

				case MainMenuAction::Statistics:
					context.stateMachine.ChangeState(std::make_unique<StatisticsState>(context));
					break;

				case MainMenuAction::Exit:
					window.close();
					break;
				}

				break;
			}
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				context.audioPlayer.Play(Assets::SoundID::MenuItemPressed);

				switch (menu.GetSelectedAction())
				{
				case MainMenuAction::StartGame:
					break;

				case MainMenuAction::Options:
					break;

				case MainMenuAction::Statistics:
					context.stateMachine.ChangeState(std::make_unique<StatisticsState>(context));
					break;

				case MainMenuAction::Exit:
					window.close();
					break;
				}
			}
		}
	}
}

void MainMenuState::Update(float deltaTime)
{
	const sf::Vector2i mousePixelPosition = sf::Mouse::getPosition(context.window);
	const sf::Vector2f mouseWorldPosition = context.window.mapPixelToCoords(mousePixelPosition);

	if (menu.UpdateMouseHover(mouseWorldPosition))
	{
		context.audioPlayer.Restart(Assets::SoundID::MenuItemSelected);
	}
}

void MainMenuState::Render(sf::RenderWindow& window)
{
	titleLabel.Render(window);
	menu.Render(window);
}