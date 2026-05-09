#pragma once

#include <unordered_map>
#include <filesystem>
#include <stdexcept>

#include <SFML/Graphics/Shader.hpp>

#include "Assets.h"

class ShaderManager
{
private:
	std::unordered_map<Assets::ShaderID, sf::Shader> shaders;

public:
	void Load(
		Assets::ShaderID id,
		const std::filesystem::path& filepath,
		sf::Shader::Type type
	);

	sf::Shader& Get(Assets::ShaderID id);

	const sf::Shader& Get(Assets::ShaderID id) const;
};