#pragma once

namespace Assets
{
	enum class FontID
	{
		Main
	};

	enum class MusicID
	{
		MainMenu,
		Gameplay,
		GameOver
	};

	enum class SoundID
	{
		MenuItemSelected,
		MenuItemPressed,

		DropPiece,
		MovePiece,
		NextLevel,
		PieceHitWall,
		RotatePiece,
		RowCleared
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
			inline constexpr const char* Gameplay = "assets/music/gameplay_music.ogg";
			inline constexpr const char* GameOver = "assets/music/game_over_music.ogg";
		}

		namespace Sounds
		{
			inline constexpr const char* MenuItemSelected = "assets/sounds/menu_item_selected.ogg";
			inline constexpr const char* MenuItemPressed = "assets/sounds/menu_item_pressed.ogg";

			inline constexpr const char* DropPiece = "assets/sounds/drop_piece.ogg";
			inline constexpr const char* MovePiece = "assets/sounds/move_piece.ogg";
			inline constexpr const char* NextLevel = "assets/sounds/next_level.ogg";
			inline constexpr const char* PieceHitWall = "assets/sounds/piece_hit_wall.ogg";
			inline constexpr const char* RotatePiece = "assets/sounds/rotate_piece.ogg";
			inline constexpr const char* RowCleared = "assets/sounds/row_cleared.ogg";
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