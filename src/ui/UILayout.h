#pragma once

#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "UIElement.h"

class UILayout : public UIElement
{
public:
    enum class Orientation
    {
        Horizontal,
        Vertical
    };

    enum class Alignment
    {
        Start,
        Center,
        End
    };

private:
    std::vector<std::unique_ptr<UIElement>> children;
    Orientation orientation;
    Alignment alignment = Alignment::Start;

    sf::Vector2f position{ 0.f, 0.f };
    float gap = 0.f;

    void UpdateLayout();

public:
    explicit UILayout(Orientation orientation);

    void Add(std::unique_ptr<UIElement> element);
    void SetGap(float gap);
    void SetAlignment(Alignment alignment);
    void SetPosition(sf::Vector2f position) override;
    [[nodiscard]] sf::Vector2f GetPosition() const override;
    [[nodiscard]] sf::Vector2f GetSize() const override;
    void Render(sf::RenderWindow& window) const override;
};