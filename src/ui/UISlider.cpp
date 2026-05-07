#include "UISlider.h"

#include <algorithm>
#include <cmath>

UISlider::UISlider(
	sf::Vector2f trackSize,
	sf::Vector2f handleSize,
	float minimum,
	float maximum,
	float value,
	float step,
	Visual trackVisual,
	Visual fillVisual,
	Visual handleVisual)
	: trackSize(trackSize)
	, handleSize(handleSize)
	, minimum(std::min(minimum, maximum))
	, maximum(std::max(minimum, maximum))
	, value(value)
	, step(std::max(0.f, step))
	, trackVisual(std::move(trackVisual))
	, fillVisual(std::move(fillVisual))
	, handleVisual(std::move(handleVisual))
{
	SetValue(value);
}

void UISlider::SetValue(float value)
{
	value = std::clamp(value, minimum, maximum);

	if (step > 0.f)
	{
		value = minimum + std::round((value - minimum) / step) * step;
		value = std::clamp(value, minimum, maximum);
	}

	this->value = value;
}

void UISlider::Increase()
{
	if (step > 0.f)
	{
		SetValue(value + step);
	}
}

void UISlider::Decrease()
{
	if (step > 0.f)
	{
		SetValue(value - step);
	}
}

void UISlider::SetRange(float minimum, float maximum)
{
	this->minimum = std::min(minimum, maximum);
	this->maximum = std::max(minimum, maximum);

	SetValue(value);
}

void UISlider::SetStep(float step)
{
	this->step = std::max(0.f, step);
	SetValue(value);
}

float UISlider::GetValue() const
{
	return value;
}

float UISlider::GetMinimum() const
{
	return minimum;
}

float UISlider::GetMaximum() const
{
	return maximum;
}

float UISlider::GetStep() const
{
	return step;
}

void UISlider::SetPosition(sf::Vector2f position)
{
	this->position = position;
}

sf::Vector2f UISlider::GetPosition() const
{
	return position;
}

sf::Vector2f UISlider::GetSize() const
{
	return
	{
		std::max(trackSize.x, handleSize.x),
		std::max(trackSize.y, handleSize.y)
	};
}

float UISlider::GetNormalizedValue() const
{
	const float range = maximum - minimum;

	if (range <= 0.f)
	{
		return 0.f;
	}

	return (value - minimum) / range;
}

void UISlider::Render(sf::RenderWindow& window) const
{
	DrawVisual(window, trackVisual, position, trackSize);
	DrawVisual(window, fillVisual, position, { trackSize.x * GetNormalizedValue(),trackSize.y });
	DrawVisual(window, handleVisual, GetHandlePosition(), handleSize);
}

sf::Vector2f UISlider::GetHandlePosition() const
{
	const float usableWidth = trackSize.x - handleSize.x;

	return
	{
		position.x + usableWidth * GetNormalizedValue(),
		position.y + (trackSize.y - handleSize.y) / 2.f
	};
}

void UISlider::DrawVisual(sf::RenderWindow& window, const Visual& visual, sf::Vector2f position, sf::Vector2f size) const
{
	// =========================
	// Rectangle visual
	// =========================

	if (std::holds_alternative<RectangleVisual>(visual))
	{
		const RectangleVisual& rectangle = std::get<RectangleVisual>(visual);
		sf::RectangleShape shape(size);
		shape.setPosition(position);
		shape.setFillColor(rectangle.fillColor);
		shape.setOutlineColor(rectangle.outlineColor);
		shape.setOutlineThickness(rectangle.outlineThickness);

		window.draw(shape);
		return;
	}

	// =========================
	// Sprite visual
	// =========================

	const SpriteVisual& spriteVisual = std::get<SpriteVisual>(visual);
	const sf::Sprite& originalSprite = spriteVisual.sprite;
	const sf::FloatRect bounds = originalSprite.getLocalBounds();

	if (bounds.size.x <= 0.f || bounds.size.y <= 0.f)
	{
		return;
	}

	// =========================
	// Stretch mode
	// =========================

	if (!spriteVisual.tiled)
	{
		sf::Sprite sprite = originalSprite;
		sprite.setPosition(position);
		sprite.setScale({ size.x / bounds.size.x, size.y / bounds.size.y });

		window.draw(sprite);

		return;
	}

	// =========================
	// Tiled mode
	// =========================

	float currentX = 0.f;

	while (currentX < size.x)
	{
		const float segmentWidth = std::min(bounds.size.x, size.x - currentX);

		sf::Sprite sprite = originalSprite;
		sprite.setPosition({ position.x + currentX, position.y });

		sf::IntRect textureRect = sprite.getTextureRect();
		textureRect.size.x = static_cast<int>(segmentWidth);

		sprite.setTextureRect(textureRect);
		sprite.setScale({ 1.f, size.y / bounds.size.y });

		window.draw(sprite);

		currentX += segmentWidth;
	}
}