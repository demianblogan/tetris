#pragma once

#include <SFML/Graphics/Text.hpp>

#include "Context.h"
#include "State.h"
#include "Label.h"
#include "VerticalMenu.h"

class MainMenuState : public State
{
private:
    Context& context;

    Label titleLabel;
    VerticalMenu menu;

public:
    MainMenuState(Context& context);

    void ProcessEvents(sf::RenderWindow& window) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
};