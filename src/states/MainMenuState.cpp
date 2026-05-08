#include "MainMenuState.h"

#include <optional>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../audio/AudioPlayer.h"
#include "../core/StateMachine.h"
#include "../resources/Assets.h"
#include "SettingsState.h"
#include "StatisticsState.h"
#include "GameState.h"

namespace
{
	constexpr float TopSpacing = 100.f;
	constexpr float TitleMenuSpacing = 90.f;
	constexpr float MenuGap = 30.f;
	constexpr float ButtonWidth = 500.f;
	constexpr float ButtonHeight = 120.f;
	constexpr unsigned int TitleSize = 300;
	constexpr unsigned int ButtonTextSize = 80;
}

MainMenuState::MainMenuState(Context& context)
	: context(context)
	, rootLayout(UI::Layout::Orientation::Vertical)
{
	rootLayout.SetHorizontalAlignment(UI::Layout::Alignment::Center);
	rootLayout.SetVerticalAlignment(UI::Layout::Alignment::Start);
	rootLayout.SetGap(0.f);

	// =====================================================
	// Top spacer
	// =====================================================

	rootLayout.Add(std::make_unique<UI::Spacer>(sf::Vector2f{ 0.f, TopSpacing }));

	// =====================================================
	// Title
	// =====================================================

	{
		auto title = std::make_unique<UI::Label>(
			context.fonts.Get(Assets::FontID::Main),
			"Tetris",
			TitleSize
		);

		title->SetFillColor(sf::Color::White);

		rootLayout.Add(std::move(title));
	}

	// =====================================================
	// Spacer between title and menu
	// =====================================================

	rootLayout.Add(std::make_unique<UI::Spacer>(sf::Vector2f{ 0.f, TitleMenuSpacing }));

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

	// =====================================================
	// Buttons
	// =====================================================

	CreateMenuButton("Start Game", MenuAction::StartGame);
	CreateMenuButton("Options", MenuAction::Options);
	CreateMenuButton("Statistics", MenuAction::Statistics);
	CreateMenuButton("Exit", MenuAction::Exit);

	UpdateSelection();
	UpdateLayout();

	sf::Music& music = context.music.Get(Assets::MusicID::MainMenu);
	music.setLooping(true);

	if (music.getStatus() != sf::Music::Status::Playing)
	{
		music.play();
	}
}

void MainMenuState::CreateMenuButton(const sf::String& text, MenuAction action)
{
	auto button = std::make_unique<UI::Button>(sf::Vector2f{ ButtonWidth, ButtonHeight });

	button->SetLabel(std::make_unique<UI::Label>(
		context.fonts.Get(Assets::FontID::Main),
		text,
		ButtonTextSize)
	);

	button->SetNormalStyle({ .backgroundColor = sf::Color(60, 60, 60), .textColor = sf::Color::White });
	button->SetSelectedStyle({ .backgroundColor = sf::Color::White,	.textColor = sf::Color::Black });

	UI::Button* buttonPointer = button.get();
	menuLayout->Add(std::move(button));
	buttons.push_back({ .button = buttonPointer, .action = action });
}

void MainMenuState::ProcessEvents(sf::RenderWindow& window
)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else if (const auto* resized = event->getIf<sf::Event::Resized>())
		{
			sf::View view = window.getView();

			view.setSize(
				{
					static_cast<float>(resized->size.x),
					static_cast<float>(resized->size.y)
				}
			);

			view.setCenter(
				{
					static_cast<float>(resized->size.x) / 2.f,
					static_cast<float>(resized->size.y) / 2.f
				}
			);

			window.setView(view);

			UpdateLayout();
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
	}
}

void MainMenuState::Update(float deltaTime)
{
	// No code
}

void MainMenuState::Render(sf::RenderWindow& window)
{
	rootLayout.Render(window);
}

void MainMenuState::UpdateLayout()
{
	const sf::Vector2f viewSize = context.window.getView().getSize();
	rootLayout.Arrange({ 0.f, 0.f }, viewSize);
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
		buttons[i].button->SetSelected(i == static_cast<std::size_t>(selectedIndex));
	}
}

void MainMenuState::ActivateSelectedButton()
{
	switch (buttons[selectedIndex].action)
	{
	case MenuAction::StartGame:
		context.stateMachine.ChangeState(std::make_unique<GameState>(context));
		break;

	case MenuAction::Options:
		context.stateMachine.ChangeState(std::make_unique<SettingsState>(context));
		break;

	case MenuAction::Statistics:
		context.stateMachine.ChangeState(std::make_unique<StatisticsState>(context));
		break;

	case MenuAction::Exit:
		context.window.close();
		break;
	}
}