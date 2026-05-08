#pragma once

enum class BlockRenderStyle
{
    WithOutline,
    WithoutOutline
};

struct GameSettings
{
    // --- Graphics:

    bool verticalSyncEnabled = true;
    unsigned int frameRateLimit = 0;
    BlockRenderStyle blockRenderStyle = BlockRenderStyle::WithOutline;

	// --- Audio:

    unsigned int soundVolume = 10;
    unsigned int musicVolume = 10;
};