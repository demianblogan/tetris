#pragma once

#include <vector>
#include "../core/Context.h"
#include "../core/State.h"
#include "../ui/UIButton.h"
#include "../ui/UILabel.h"
#include "../ui/UILayout.h"

class MainMenuState : public State
{
private:
    enum class MenuAction
    {
        StartGame,
        Options,
        Statistics,
        Exit
    };

    struct MenuButton
    {
        UIButton* button;
        MenuAction action;
    };

    Context& context;

    UILabel titleLabel;
    UILayout menuLayout;

    std::vector<MenuButton> buttons;

    int selectedIndex = 0;

public:
    MainMenuState(Context& context);

    void ProcessEvents(sf::RenderWindow& window) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;

private:
    void SelectPreviousMenuItem();
    void SelectNextMenuItem();

    void UpdateSelection();

    void ActivateSelectedButton();
};