#pragma once

namespace Assets
{
	enum class FontID
	{
		Main
	};

	enum class MusicID
	{
		MainMenu
	};

	enum class SoundID
	{
		MenuItemSelected,
		MenuItemPressed
	};

	enum class TextureID
	{
		BlockSpritesheetWithOutline,
		BlockSpritesheetWithoutOutline
	};

	namespace Paths
	{
		namespace Fonts
		{
			inline constexpr const char* Main = "assets/fonts/main.ttf";
		}

		namespace Music
		{
			inline constexpr const char* MainMenu = "assets/music/main_menu_music.ogg";
		}

		namespace Sounds
		{
			inline constexpr const char* MenuItemSelected = "assets/sounds/menu_item_selected.ogg";
			inline constexpr const char* MenuItemPressed = "assets/sounds/menu_item_pressed.ogg";
		}

		namespace Textures
		{
			inline constexpr const char* BlockSpritesheetWithOutline = "assets/sprites/block_spritesheet_with_outline.png";
			inline constexpr const char* BlockSpritesheetWithoutOutline = "assets/sprites/block_spritesheet_without_outline.png";
		}
	}
}

namespace Data
{
	namespace Paths
	{
		inline constexpr const char* Scores = "data/scores.txt";
		inline constexpr const char* Settings = "data/settings.txt";
	}
}