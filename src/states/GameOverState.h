#pragma once

#include <vector>

#include "../core/Context.h"
#include "../core/State.h"

#include "../ui/Button.h"
#include "../ui/Layout.h"
#include "../ui/Label.h"
#include "../ui/Spacer.h"

class GameOverState final : public State
{
private:
	enum class MenuAction
	{
		SaveRecord,
		RestartGame,
		MainMenu
	};

	struct MenuButton
	{
		UI::Button* button = nullptr;
		MenuAction action;
	};

	static constexpr std::size_t MAX_NAME_LENGTH = 20;

	Context& context;

	UI::Layout rootLayout;
	UI::Layout* menuLayout = nullptr;

	std::vector<MenuButton> buttons;

	UI::Label* scoreLabel = nullptr;
	UI::Label* highScoreLabel = nullptr;
	UI::Label* playerNameLabel = nullptr;

	int selectedIndex = 0;
	int finalScore = 0;
	bool isHighScore = false;
	sf::String playerName;

private:
	void CreateMenuButton(const sf::String& text, MenuAction action);

	void SelectPreviousMenuItem();
	void SelectNextMenuItem();

	void UpdateSelection();
	void UpdateLayout();
	void ActivateSelectedButton();

	[[nodiscard]] bool IsPlayerNameValid() const;

public:
	GameOverState(Context& context, int finalScore);

	void ProcessEvents(sf::RenderWindow& window) override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;
};