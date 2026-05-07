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

	namespace Paths
	{
		namespace Fonts
		{
			inline constexpr const char* Main = "assets/fonts/jacquard24.ttf";
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