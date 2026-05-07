#pragma once

#include <filesystem>

#include "GameSettings.h"

struct Context;

class SettingsManager
{
private:
    GameSettings settings;
    std::filesystem::path filepath;

public:
    SettingsManager(const std::filesystem::path& filepath);

    void Load();
    void Save() const;

    void Apply(Context& context) const;

    GameSettings& GetSettings();
    const GameSettings& GetSettings() const;
};